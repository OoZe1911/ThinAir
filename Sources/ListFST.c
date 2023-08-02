// List all files in a NGC ISO file.
// Usage :
// ListFST <filename.gcm>

/*
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char * argv[])
{
   unsigned char   buffer[256];
   unsigned char   *FST;
   unsigned long   fst_offset=0,filename_offset=0,entries=0,string_table_offset=0,fst_size=0;
   unsigned int   file=0,found=0;
   unsigned long   i=0,j=0,k=0,offset=0,parent_offset=0,next_offset=0,name_offset=0;
   unsigned char   *filename=malloc(30),*path=malloc(256),*temp=malloc(256),*current_path=malloc(256);

   if(argc!=2)
   {
      printf("List FST usage :\n");
      printf("%s <filename.gcm>\n",argv[0]);
   }
   else
   {
      strcpy(filename,"");
      strcpy(path,"");
      strcpy(current_path,"/");
      file=open(argv[1],O_RDONLY);
      if (file==0)
      {
         printf("error opening %s file.\n",argv[1]);
         exit(-1);
      }

      read(file, buffer, 256);
      buffer[255]=0;
      printf("%c%c%c (",buffer[0],buffer[1],buffer[2]);
      switch(buffer[3]){
         case 'E':
            printf("USA/NTSC");
            break;
         case 'P':
            printf("Europe/PAL");
            break;
         default:   
            printf("Japan/NTSC");
      }
      printf(") - %s\n",buffer+32);

      // Load all the FST in memory :
      lseek(file,0x424,SEEK_SET);
      read(file,buffer,8);
      fst_offset=(unsigned int)buffer[0]*256*256*256+(unsigned int)buffer[1]*256*256+(unsigned int)buffer[2]*256+(unsigned int)buffer[3];
//      printf("Fst offset=%ld - ",fst_offset);
      fst_size=(unsigned int)buffer[4]*256*256*256+(unsigned int)buffer[5]*256*256+(unsigned int)buffer[6]*256+(unsigned int)buffer[7];
      FST=malloc(fst_size);
      lseek(file,fst_offset,SEEK_SET);
      read(file,FST,fst_size);
      close(file);

      entries=(unsigned int)FST[8]*256*256*256+(unsigned int)FST[9]*256*256+(unsigned int)FST[10]*256+(unsigned int)FST[11];
      string_table_offset=12*entries;
//      printf("Number of FST entries : %i\n",entries);

      for (i=1;i<entries;i++)
      {
         offset=i*0x0c;
         if(FST[offset]==0)
         {
            filename_offset=(unsigned int)FST[offset+1]*256*256+(unsigned int)FST[offset+2]*256+(unsigned int)FST[offset+3];
            strcpy(filename,&FST[string_table_offset+filename_offset]);

            // Looking for the full path of the file :
            strcpy(path,"");
            strcpy(temp,"");
            j=0x0c;
            found=0;
            while ( (j<offset) && (found==0) )
            {
               // Does the file appear in a "next offset" reference ?
                  // (search starts from the begining of the FST)
               if(FST[j]==1)
               {
                  next_offset=(unsigned int)FST[j+8]*256*256*256+(unsigned int)FST[j+9]*256*256+(unsigned int)FST[j+10]*256+(unsigned int)FST[j+11];
                  if (next_offset==i)
                  {
                     parent_offset=(unsigned int)FST[j+4]*256*256*256+(unsigned int)FST[j+5]*256*256+(unsigned int)FST[j+6]*256+(unsigned int)FST[j+7];
                     while(parent_offset!=0)
                     {
                        // Folder's name :
                        name_offset=(unsigned int)FST[parent_offset*0x0c+1]*256*256+(unsigned int)FST[parent_offset*0x0c+2]*256+(unsigned int)FST[parent_offset*0x0c+3];
                        strcpy(temp,&FST[string_table_offset+name_offset]);
                        strcat(temp,"/");
                        strcat(temp,path);
                        strcpy(path,temp);

                        parent_offset=(unsigned int)FST[parent_offset*0x0c+4]*256*256*256+(unsigned int)FST[parent_offset*0x0c+5]*256*256+(unsigned int)FST[parent_offset*0x0c+6]*256+(unsigned int)FST[parent_offset*0x0c+7];
                     }
                     strcpy(temp,"/");
                     strcat(temp,path);
                     strcpy(path,temp);
                     found=1;
                  }
               }
               j=j+0x0c;
            }
            // If the file is not in a "next offset" reference
            //      check the if the previous entry is not a directory
            //      if YES path=path of the previous entry (which is a folder)
            //      if NO path=curent path (=the path of the previous file)
            // else
            //      path=path previously found
            if(found==1)
            {
               strcpy(current_path,path);
            }
            else
            {
               if ((offset>0x0c) && (FST[offset-0x0c]==1))
               {
                  parent_offset=i-1;
                  while(parent_offset!=0)
                  {
                     // Folder's name :
                     name_offset=(unsigned int)FST[parent_offset*0x0c+1]*256*256+(unsigned int)FST[parent_offset*0x0c+2]*256+(unsigned int)FST[parent_offset*0x0c+3];
                     strcpy(temp,&FST[string_table_offset+name_offset]);
                     strcat(temp,"/");
                     strcat(temp,path);
                     strcpy(path,temp);

                     parent_offset=(unsigned int)FST[parent_offset*0x0c+4]*256*256*256+(unsigned int)FST[parent_offset*0x0c+5]*256*256+(unsigned int)FST[parent_offset*0x0c+6]*256+(unsigned int)FST[parent_offset*0x0c+7];
                  }
                  strcpy(temp,"/");
                  strcat(temp,path);
                  strcpy(path,temp);
                  strcpy(current_path,path);
               }
            }
            printf("%s%s\n",current_path,filename);
         }
      }
   }
} 