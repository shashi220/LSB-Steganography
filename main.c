#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc,char *argv[])
{

    EncodeInfo encInfo;
    /* enabeling the decoding */
    DecodeInfo decInfo;
    int ret=check_operation_type(argv);
    if(argc == 1 )
    {
	printf("please pass arguments through command line\n");
    }

    if(ret==e_encode)
    {
	printf("<-----------Encoding is selected----------->\n");
	if(read_and_validate_encode_args(argv,&encInfo)==e_success)
	{
	    printf("read and validation succcess\n");
	    printf("<----------Started encoding----------->\n");
	    if(do_encoding(&encInfo)==e_success)
	    {
		printf("<--------Encoding completed---------->\n");
	    }
	    else
	    {
		printf("Encoding failed\n");
	    }
	}
	else
	{
	    printf("read and validation failed\n");
	}
    }
    else if(check_operation_type(argv) == e_decode)
    {
	printf("<-------------Selected decoding------------>\n");
	/* checking the read and validation  for decoding */
	if(read_and_validate_decode_args(argv, &decInfo) == d_success)
	{
	    printf("read and validation of decode is success\n");
	    printf("<------------------Started Decoding----------------->\n");
	    /* decoding part */
	    if(do_decoding(argv, &decInfo)== d_success)
	    {
		printf("<-------------------Decoded done------------------->\n");
	    }
	    else
	    {
		printf("Failed to decode\n");
	    }
	}
	else
	{
	    printf("read and validation is failed\n");
	}
    }
    return 0;
}
