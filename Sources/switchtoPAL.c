// Generated code

#include <stdio.h>

int main()
{
	FILE *fileRip=fopen("riptool.dol","rb+");
	FILE *fileAcl=fopen("s3-acl.dol","rb+");
	FILE *fileLoader=fopen("s3-l.dol","rb+");

	printf("Patching riptool.dol...\n");

	fseek(fileRip,16368,SEEK_SET);
	fputc((char)17,fileRip);
	fseek(fileRip,16369,SEEK_SET);
	fputc((char)245,fileRip);
	fseek(fileRip,16370,SEEK_SET);
	fputc((char)1,fileRip);
	fseek(fileRip,16372,SEEK_SET);
	fputc((char)75,fileRip);
	fseek(fileRip,16373,SEEK_SET);
	fputc((char)106,fileRip);
	fseek(fileRip,16375,SEEK_SET);
	fputc((char)176,fileRip);
	fseek(fileRip,16377,SEEK_SET);
	fputc((char)248,fileRip);
	fseek(fileRip,16378,SEEK_SET);
	fputc((char)86,fileRip);
	fseek(fileRip,16381,SEEK_SET);
	fputc((char)1,fileRip);
	fseek(fileRip,16383,SEEK_SET);
	fputc((char)35,fileRip);
	fseek(fileRip,16385,SEEK_SET);
	fputc((char)0,fileRip);
	fseek(fileRip,16387,SEEK_SET);
	fputc((char)36,fileRip);
	fseek(fileRip,16388,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16389,SEEK_SET);
	fputc((char)43,fileRip);
	fseek(fileRip,16390,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16391,SEEK_SET);
	fputc((char)109,fileRip);
	fseek(fileRip,16392,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16393,SEEK_SET);
	fputc((char)138,fileRip);
	fseek(fileRip,16394,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16395,SEEK_SET);
	fputc((char)76,fileRip);
	fseek(fileRip,16396,SEEK_SET);
	fputc((char)0,fileRip);
	fseek(fileRip,16397,SEEK_SET);
	fputc((char)67,fileRip);
	fseek(fileRip,16398,SEEK_SET);
	fputc((char)90,fileRip);
	fseek(fileRip,16399,SEEK_SET);
	fputc((char)78,fileRip);
	fseek(fileRip,16404,SEEK_SET);
	fputc((char)0,fileRip);
	fseek(fileRip,16405,SEEK_SET);
	fputc((char)67,fileRip);
	fseek(fileRip,16406,SEEK_SET);
	fputc((char)90,fileRip);
	fseek(fileRip,16407,SEEK_SET);
	fputc((char)78,fileRip);
	fseek(fileRip,16412,SEEK_SET);
	fputc((char)1,fileRip);
	fseek(fileRip,16413,SEEK_SET);
	fputc((char)60,fileRip);
	fseek(fileRip,16414,SEEK_SET);
	fputc((char)1,fileRip);
	fseek(fileRip,16415,SEEK_SET);
	fputc((char)68,fileRip);
	fseek(fileRip,16417,SEEK_SET);
	fputc((char)57,fileRip);
	fseek(fileRip,16419,SEEK_SET);
	fputc((char)177,fileRip);
	fseek(fileRip,16624,SEEK_SET);
	fputc((char)17,fileRip);
	fseek(fileRip,16625,SEEK_SET);
	fputc((char)245,fileRip);
	fseek(fileRip,16626,SEEK_SET);
	fputc((char)1,fileRip);
	fseek(fileRip,16628,SEEK_SET);
	fputc((char)75,fileRip);
	fseek(fileRip,16629,SEEK_SET);
	fputc((char)106,fileRip);
	fseek(fileRip,16631,SEEK_SET);
	fputc((char)176,fileRip);
	fseek(fileRip,16633,SEEK_SET);
	fputc((char)248,fileRip);
	fseek(fileRip,16634,SEEK_SET);
	fputc((char)86,fileRip);
	fseek(fileRip,16637,SEEK_SET);
	fputc((char)1,fileRip);
	fseek(fileRip,16639,SEEK_SET);
	fputc((char)35,fileRip);
	fseek(fileRip,16641,SEEK_SET);
	fputc((char)0,fileRip);
	fseek(fileRip,16643,SEEK_SET);
	fputc((char)36,fileRip);
	fseek(fileRip,16644,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16645,SEEK_SET);
	fputc((char)43,fileRip);
	fseek(fileRip,16646,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16647,SEEK_SET);
	fputc((char)109,fileRip);
	fseek(fileRip,16648,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16649,SEEK_SET);
	fputc((char)138,fileRip);
	fseek(fileRip,16650,SEEK_SET);
	fputc((char)77,fileRip);
	fseek(fileRip,16651,SEEK_SET);
	fputc((char)76,fileRip);
	fseek(fileRip,16668,SEEK_SET);
	fputc((char)1,fileRip);
	fseek(fileRip,16669,SEEK_SET);
	fputc((char)60,fileRip);
	fseek(fileRip,16671,SEEK_SET);
	fputc((char)68,fileRip);
	fseek(fileRip,16673,SEEK_SET);
	fputc((char)57,fileRip);
	fseek(fileRip,16675,SEEK_SET);
	fputc((char)177,fileRip);

	printf("Patching s3-acl.dol...\n");

	fseek(fileAcl,685016,SEEK_SET);
	fputc((char)17,fileAcl);
	fseek(fileAcl,685017,SEEK_SET);
	fputc((char)245,fileAcl);
	fseek(fileAcl,685018,SEEK_SET);
	fputc((char)1,fileAcl);
	fseek(fileAcl,685020,SEEK_SET);
	fputc((char)75,fileAcl);
	fseek(fileAcl,685021,SEEK_SET);
	fputc((char)106,fileAcl);
	fseek(fileAcl,685023,SEEK_SET);
	fputc((char)176,fileAcl);
	fseek(fileAcl,685025,SEEK_SET);
	fputc((char)248,fileAcl);
	fseek(fileAcl,685026,SEEK_SET);
	fputc((char)86,fileAcl);
	fseek(fileAcl,685029,SEEK_SET);
	fputc((char)1,fileAcl);
	fseek(fileAcl,685031,SEEK_SET);
	fputc((char)35,fileAcl);
	fseek(fileAcl,685033,SEEK_SET);
	fputc((char)0,fileAcl);
	fseek(fileAcl,685035,SEEK_SET);
	fputc((char)36,fileAcl);
	fseek(fileAcl,685036,SEEK_SET);
	fputc((char)77,fileAcl);
	fseek(fileAcl,685037,SEEK_SET);
	fputc((char)43,fileAcl);
	fseek(fileAcl,685038,SEEK_SET);
	fputc((char)77,fileAcl);
	fseek(fileAcl,685039,SEEK_SET);
	fputc((char)109,fileAcl);
	fseek(fileAcl,685040,SEEK_SET);
	fputc((char)77,fileAcl);
	fseek(fileAcl,685041,SEEK_SET);
	fputc((char)138,fileAcl);
	fseek(fileAcl,685042,SEEK_SET);
	fputc((char)77,fileAcl);
	fseek(fileAcl,685043,SEEK_SET);
	fputc((char)76,fileAcl);
	fseek(fileAcl,685045,SEEK_SET);
	fputc((char)67,fileAcl);
	fseek(fileAcl,685046,SEEK_SET);
	fputc((char)90,fileAcl);
	fseek(fileAcl,685047,SEEK_SET);
	fputc((char)78,fileAcl);
	fseek(fileAcl,685053,SEEK_SET);
	fputc((char)67,fileAcl);
	fseek(fileAcl,685054,SEEK_SET);
	fputc((char)90,fileAcl);
	fseek(fileAcl,685055,SEEK_SET);
	fputc((char)78,fileAcl);
	fseek(fileAcl,685060,SEEK_SET);
	fputc((char)1,fileAcl);
	fseek(fileAcl,685061,SEEK_SET);
	fputc((char)60,fileAcl);
	fseek(fileAcl,685062,SEEK_SET);
	fputc((char)1,fileAcl);
	fseek(fileAcl,685063,SEEK_SET);
	fputc((char)68,fileAcl);
	fseek(fileAcl,685065,SEEK_SET);
	fputc((char)57,fileAcl);
	fseek(fileAcl,685067,SEEK_SET);
	fputc((char)177,fileAcl);

	printf("Patching s3-l.dol...\n");

	fseek(fileLoader,50684,SEEK_SET);
	fputc((char)17,fileLoader);
	fseek(fileLoader,50685,SEEK_SET);
	fputc((char)245,fileLoader);
	fseek(fileLoader,50686,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,50688,SEEK_SET);
	fputc((char)75,fileLoader);
	fseek(fileLoader,50689,SEEK_SET);
	fputc((char)106,fileLoader);
	fseek(fileLoader,50691,SEEK_SET);
	fputc((char)176,fileLoader);
	fseek(fileLoader,50693,SEEK_SET);
	fputc((char)248,fileLoader);
	fseek(fileLoader,50694,SEEK_SET);
	fputc((char)86,fileLoader);
	fseek(fileLoader,50697,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,50699,SEEK_SET);
	fputc((char)35,fileLoader);
	fseek(fileLoader,50701,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,50703,SEEK_SET);
	fputc((char)36,fileLoader);
	fseek(fileLoader,50704,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50705,SEEK_SET);
	fputc((char)43,fileLoader);
	fseek(fileLoader,50706,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50707,SEEK_SET);
	fputc((char)109,fileLoader);
	fseek(fileLoader,50708,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50709,SEEK_SET);
	fputc((char)138,fileLoader);
	fseek(fileLoader,50710,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50711,SEEK_SET);
	fputc((char)76,fileLoader);
	fseek(fileLoader,50712,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,50713,SEEK_SET);
	fputc((char)67,fileLoader);
	fseek(fileLoader,50714,SEEK_SET);
	fputc((char)90,fileLoader);
	fseek(fileLoader,50715,SEEK_SET);
	fputc((char)78,fileLoader);
	fseek(fileLoader,50720,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,50721,SEEK_SET);
	fputc((char)67,fileLoader);
	fseek(fileLoader,50722,SEEK_SET);
	fputc((char)90,fileLoader);
	fseek(fileLoader,50723,SEEK_SET);
	fputc((char)78,fileLoader);
	fseek(fileLoader,50728,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,50729,SEEK_SET);
	fputc((char)60,fileLoader);
	fseek(fileLoader,50730,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,50731,SEEK_SET);
	fputc((char)68,fileLoader);
	fseek(fileLoader,50733,SEEK_SET);
	fputc((char)57,fileLoader);
	fseek(fileLoader,50735,SEEK_SET);
	fputc((char)177,fileLoader);
	fseek(fileLoader,50940,SEEK_SET);
	fputc((char)17,fileLoader);
	fseek(fileLoader,50941,SEEK_SET);
	fputc((char)245,fileLoader);
	fseek(fileLoader,50942,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,50944,SEEK_SET);
	fputc((char)75,fileLoader);
	fseek(fileLoader,50945,SEEK_SET);
	fputc((char)106,fileLoader);
	fseek(fileLoader,50947,SEEK_SET);
	fputc((char)176,fileLoader);
	fseek(fileLoader,50949,SEEK_SET);
	fputc((char)248,fileLoader);
	fseek(fileLoader,50950,SEEK_SET);
	fputc((char)86,fileLoader);
	fseek(fileLoader,50953,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,50955,SEEK_SET);
	fputc((char)35,fileLoader);
	fseek(fileLoader,50957,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,50959,SEEK_SET);
	fputc((char)36,fileLoader);
	fseek(fileLoader,50960,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50961,SEEK_SET);
	fputc((char)43,fileLoader);
	fseek(fileLoader,50962,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50963,SEEK_SET);
	fputc((char)109,fileLoader);
	fseek(fileLoader,50964,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50965,SEEK_SET);
	fputc((char)138,fileLoader);
	fseek(fileLoader,50966,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,50967,SEEK_SET);
	fputc((char)76,fileLoader);
	fseek(fileLoader,50984,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,50985,SEEK_SET);
	fputc((char)60,fileLoader);
	fseek(fileLoader,50987,SEEK_SET);
	fputc((char)68,fileLoader);
	fseek(fileLoader,50989,SEEK_SET);
	fputc((char)57,fileLoader);
	fseek(fileLoader,50991,SEEK_SET);
	fputc((char)177,fileLoader);
	fseek(fileLoader,51068,SEEK_SET);
	fputc((char)17,fileLoader);
	fseek(fileLoader,51069,SEEK_SET);
	fputc((char)245,fileLoader);
	fseek(fileLoader,51070,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,51072,SEEK_SET);
	fputc((char)75,fileLoader);
	fseek(fileLoader,51073,SEEK_SET);
	fputc((char)106,fileLoader);
	fseek(fileLoader,51075,SEEK_SET);
	fputc((char)176,fileLoader);
	fseek(fileLoader,51077,SEEK_SET);
	fputc((char)248,fileLoader);
	fseek(fileLoader,51078,SEEK_SET);
	fputc((char)86,fileLoader);
	fseek(fileLoader,51081,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,51083,SEEK_SET);
	fputc((char)35,fileLoader);
	fseek(fileLoader,51085,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,51087,SEEK_SET);
	fputc((char)36,fileLoader);
	fseek(fileLoader,51088,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51089,SEEK_SET);
	fputc((char)43,fileLoader);
	fseek(fileLoader,51090,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51091,SEEK_SET);
	fputc((char)109,fileLoader);
	fseek(fileLoader,51092,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51093,SEEK_SET);
	fputc((char)138,fileLoader);
	fseek(fileLoader,51094,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51095,SEEK_SET);
	fputc((char)76,fileLoader);
	fseek(fileLoader,51096,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,51097,SEEK_SET);
	fputc((char)67,fileLoader);
	fseek(fileLoader,51098,SEEK_SET);
	fputc((char)90,fileLoader);
	fseek(fileLoader,51099,SEEK_SET);
	fputc((char)78,fileLoader);
	fseek(fileLoader,51104,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,51105,SEEK_SET);
	fputc((char)67,fileLoader);
	fseek(fileLoader,51106,SEEK_SET);
	fputc((char)90,fileLoader);
	fseek(fileLoader,51107,SEEK_SET);
	fputc((char)78,fileLoader);
	fseek(fileLoader,51112,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,51113,SEEK_SET);
	fputc((char)60,fileLoader);
	fseek(fileLoader,51114,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,51115,SEEK_SET);
	fputc((char)68,fileLoader);
	fseek(fileLoader,51117,SEEK_SET);
	fputc((char)57,fileLoader);
	fseek(fileLoader,51119,SEEK_SET);
	fputc((char)177,fileLoader);
	fseek(fileLoader,51324,SEEK_SET);
	fputc((char)17,fileLoader);
	fseek(fileLoader,51325,SEEK_SET);
	fputc((char)245,fileLoader);
	fseek(fileLoader,51326,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,51328,SEEK_SET);
	fputc((char)75,fileLoader);
	fseek(fileLoader,51329,SEEK_SET);
	fputc((char)106,fileLoader);
	fseek(fileLoader,51331,SEEK_SET);
	fputc((char)176,fileLoader);
	fseek(fileLoader,51333,SEEK_SET);
	fputc((char)248,fileLoader);
	fseek(fileLoader,51334,SEEK_SET);
	fputc((char)86,fileLoader);
	fseek(fileLoader,51337,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,51339,SEEK_SET);
	fputc((char)35,fileLoader);
	fseek(fileLoader,51341,SEEK_SET);
	fputc((char)0,fileLoader);
	fseek(fileLoader,51343,SEEK_SET);
	fputc((char)36,fileLoader);
	fseek(fileLoader,51344,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51345,SEEK_SET);
	fputc((char)43,fileLoader);
	fseek(fileLoader,51346,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51347,SEEK_SET);
	fputc((char)109,fileLoader);
	fseek(fileLoader,51348,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51349,SEEK_SET);
	fputc((char)138,fileLoader);
	fseek(fileLoader,51350,SEEK_SET);
	fputc((char)77,fileLoader);
	fseek(fileLoader,51351,SEEK_SET);
	fputc((char)76,fileLoader);
	fseek(fileLoader,51368,SEEK_SET);
	fputc((char)1,fileLoader);
	fseek(fileLoader,51369,SEEK_SET);
	fputc((char)60,fileLoader);
	fseek(fileLoader,51371,SEEK_SET);
	fputc((char)68,fileLoader);
	fseek(fileLoader,51373,SEEK_SET);
	fputc((char)57,fileLoader);
	fseek(fileLoader,51375,SEEK_SET);
	fputc((char)177,fileLoader);
	close(fileRip);
	close(fileAcl);
	close(fileLoader);
	return(0);
}
