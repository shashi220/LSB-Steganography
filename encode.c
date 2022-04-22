#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
#if 1
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e")==0)
	return e_encode;
    if(strcmp(argv[1],"-d")==0)
	return e_decode;
    else
	return e_unsupported;
}  
#endif

#if 1
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}
#endif
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

#if 1
Status open_files(EncodeInfo *encInfo)
{
	// Src Image file
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	// Do Error handling
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

		return e_failure;
	}

	// Secret file
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	// Do Error handling
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure;
	}

	// Stego Image file
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	// Do Error handling
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}

	// No failure return e_success
	return e_success;
}
#endif
#if 1
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
      //1. check argv[2] is .bmp file or not
      if (strstr(argv[2],".bmp"))
	  encInfo->src_image_fname=argv[2];
      else
          return e_failure;

      //2. check if argv[3] is .txt file or not
      if (strstr(argv[3],".txt"))
	  encInfo->secret_fname=argv[3];
      else
	  return e_failure;
      //3. check if argv[4] is available or not
      if (argv[4] == NULL)
      {
	  encInfo->stego_image_fname = "default_bmp";
      }
      else
      {
	  encInfo->stego_image_fname = argv[4];
      }
      encInfo -> extn = strstr(argv[3],".txt");
      return e_success;
}

#endif

#if 1
Status do_encoding(EncodeInfo *encInfo)
{

    if(open_files(encInfo) == e_success)
    {
	printf("INFO : open files SUCCESS\n");
	if(check_capacity(encInfo) == e_success)
	{
	    printf("INFO : Check capacity SUCCESS\n");
	    if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
	    {
		printf("INFO: copy BMP header success SUCCESS\n");
		if((encode_magic_string(MAGIC_STRING,encInfo))==e_success)
		{
		    printf("INFO: Magic string success\n");
		    if((secret_file_extn_size(strlen(encInfo->extn),encInfo)==e_success))
		    {
			printf("INFO: file extension size success\n");
			if(encode_secret_file_extn(encInfo->extn, encInfo) == e_success)
			{
			    printf("secret file extension is encoded\n");
			    if(encode_secret_file_size(encInfo->size_secret_file, encInfo ) == e_success)
			    {
				printf("secret file size is encoded\n");
				if(encode_secret_file_data(encInfo ) == e_success)
				{
				    printf("secret file data is encoded\n");
				    if(copy_remaining_img_data(encInfo->fptr_src_image ,encInfo->fptr_stego_image) == e_success)
				    {
					printf("remaining image data is copied\n");
					return e_success;
				    }
				    else
				    {
					printf("remaining data not copied\n");
					return -1;
				    }
				}
				else
				{
				    printf("secret file data encoding failure\n");
				    return -1;
				}
			    }
			    else
			    {
				printf("secret file size encoding failure\n");
				return -1;
			    }
			}
			else
			{
			    printf("secret file extension encoding failure");
			    return -1;
			}
		    }
		    else
		    {
			printf("Error: file extension size failure");
			return -1;
		    }
		}
		else
		{
		    printf("Error:Magic string failure");
		    return -1;
		}
	    }
	    else
	    {
		printf("ERROR : Copy BMP header Failure\n");
		return -1;
	    }
	}
	else
	    printf("ERROR : Encoding is not POSSIBLE\n");
	    return -1;
    }
    else
    {
	printf("ERROR : open files failure");
	return -1;
    }
}
#endif

//check capacity
Status check_capacity(EncodeInfo *encInfo)
{
    encInfo -> image_capacity =  get_image_size_for_bmp(encInfo -> fptr_src_image);
    encInfo -> size_secret_file = get_file_size(encInfo -> fptr_secret);
    //if(encInfo -> image_capacity > (( 2 + 4 + 4 + 4 + encInfo -> size_secret_file)*8))
    long  total_bytes = 54 + strlen(MAGIC_STRING) * 8 + 32 + 32 + 32 + encInfo->size_secret_file * 8;
    if(total_bytes <= encInfo->image_capacity)
    {
	return e_success;
    }
    else
    {
	return e_failure;
    }
}
//get file size
uint get_file_size(FILE *fptr)
{
    int size;
    fseek(fptr,0l,SEEK_END);
    size=ftell(fptr);
    rewind(fptr);
    return size;
}
#if 1
/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char str[54];
    fseek(fptr_src_image, 0L, SEEK_SET);
    fseek(fptr_dest_image, 0L, SEEK_SET);
    fread(str,54,1,fptr_src_image);
    fwrite(str,54,1,fptr_dest_image);
    return e_success;
}
#endif

#if 1
/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
    encode_data_to_image(magic_string,strlen(magic_string),encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}

/* Encode secret file extension size*/
Status secret_file_extn_size(long extn_size, EncodeInfo *encInfo)
{
    char src_data[32];
    fread(src_data,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(extn_size,src_data);
    fwrite(src_data,32,1,encInfo->fptr_stego_image);
    return e_success;
}



/*encoding the size of extension file*/
Status encode_size_to_lsb(long extn_size, char*image_buffer)
{
	for(int i=0;i<32;i++)
	{
	    if((extn_size >> (31-i)) &1)
	    {
		image_buffer[i]=image_buffer[i] | 1;
	    }
	    else
	    {
		image_buffer[i]=image_buffer[i] & ~1;
	    }
	}
}

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    encode_data_to_image(file_extn,strlen(file_extn),encInfo -> fptr_src_image, encInfo -> fptr_stego_image);
}
/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char str[32];
    fread(str,32,1,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,str);
    fwrite(str,32,1,encInfo->fptr_stego_image);
    return e_success;
}

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    fseek(encInfo->fptr_secret,0L,SEEK_SET);
    char str[encInfo->size_secret_file];
    fread(str,encInfo->size_secret_file,1,encInfo->fptr_secret);
    encode_data_to_image(str,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    return e_success;
}
/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char image_buffer[8];
    int i;
    for(i=0;i<size;i++)
    {
	fread(image_buffer,8,1,fptr_src_image);
	encode_byte_to_lsb(data[i],image_buffer);
	fwrite(image_buffer,8,1,fptr_stego_image);
    }
    return e_success;
}
#endif

#if 1
/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0; i<8 ; i++)
    {
	if((data >> (7-i))&1)
	{
	    image_buffer[i] = image_buffer[i] | 1;
	}
	else
	{
	    image_buffer[i] = image_buffer[i] & ~1; // 0 can also be used in place of ~1
	}
    }
}

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while((fread(&ch,1,1,fptr_src))>0)
    {
	fwrite(&ch,1,1,fptr_dest);
    }
    return e_success;
}

#endif
