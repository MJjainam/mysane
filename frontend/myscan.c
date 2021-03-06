#include "myscan.h"
#include <stdio.h>

#ifdef HAVE_LIBPNG
#include <png.h>
#endif

#include <jpeglib.h>

static void auth_callback(SANE_String_Const resource,
                          SANE_Char *username, SANE_Char *password)
{
}

struct Mysane bunch;
Image image;
SANE_Parameters parm;


void print_scanner_details()
{
    printf("%s\n", "Printing scanner details....");
    printf("%s ", bunch.devlist[0]->name);
    printf("%s ", bunch.devlist[0]->vendor);
    printf("%s ", bunch.devlist[0]->model);
    printf("%s\n", bunch.devlist[0]->type);
    printf("%s\n", "Printing scanner details....");
    
}


void convert_to_jpeg(){
    struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);



    FILE * outfile;
    char filename[20];
    strcat(filename, "output.jpeg");

	if ((outfile = fopen(filename, "wb")) == NULL) {
	    fprintf(stderr, "can't open %s\n", filename);
	    exit(1);
	}
    jpeg_stdio_dest(&cinfo, outfile);
    
    cinfo.image_width = parm.pixels_per_line;
    cinfo.image_height = parm.lines;
    cinfo.in_color_space = JCS_RGB;
    cinfo.input_components = 3;

    jpeg_set_defaults(&cinfo);

    jpeg_start_compress(&cinfo, TRUE);

    JSAMPROW row_pointer[1];

    int row_stride = cinfo.image_width*3;
    while (cinfo.next_scanline < cinfo.image_height) {
	    row_pointer[0] = &image.data[cinfo.next_scanline * row_stride];
	    jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    
    jpeg_finish_compress(&cinfo);

    jpeg_destroy_compress(&cinfo);
    

}

int main()
{
    int version_code;
    unsigned char buf8[2 * 32768];

    // SANE_Byte *image;
    SANE_Word total_bytes; //stores total image length
    // unsigned char *buf8ptr;
    SANE_Int len;
    SANE_Handle handle;
    bunch.dummy_filename = "dummy.in";

    SANE_Status status = sane_init(&version_code, (void *)auth_callback);
    sane_get_devices(&bunch.devlist, SANE_FALSE);

    print_scanner_details();

    status = sane_open(bunch.devlist[0]->name, &handle);
    status = sane_get_parameters(handle, &parm); //Get the parameters like number of bytes per line

    image.data = malloc(parm.bytes_per_line *parm.lines* 2 * 3);
    
    printf("The default format is %d", parm.format);

    // int color = 0;
    // char *frameType = "Color";
    SANE_String frameType= "Color";

    SANE_Option_Descriptor *sop;
   
    sop = sane_get_option_descriptor(handle,2);
    printf("sop name is %s",sop->name);
    printf("sop  title is %s",sop->title);
    printf("sop type is is %d \n",sop->type);
    printf("sop description is %s",sop->desc);
    
    
    
    for(int i=0;i<4;i++){

        printf("sop strin list is %s \n",*(sop->constraint.string_list+i));
    }


    // SANE_Int feedback;
    status = sane_control_option(handle,2,SANE_ACTION_SET_VALUE,frameType,NULL);
    printf("frametype is %s",frameType);
    if (status != SANE_STATUS_GOOD)
	{
		printf("Option did not set");
    }

    printf("\nPrinting status %d\n",status);

    status = sane_get_parameters(handle, &parm); //Get the parameters like number of bytes per line
    
    switch (parm.format)
    {
    case SANE_FRAME_RGB:
        printf("Yes its RGB\n");
        break;
    case SANE_FRAME_GRAY:
        printf("No it's gray\n");
        break;
        /* etc... */
    default:
        printf("defaultcase \n");
        break;
    }

    // printf("The default format is %d",parm.format);

    // sane_control_option(handle,)
    status = sane_start(handle);

    bunch.out = fopen(bunch.dummy_filename, "wb+");
    int offset = 0;
    while (1)
    {

        status = sane_read(handle, (SANE_Byte *)buf8, sizeof(buf8), &len);
        printf("Length of the image %d\n", len);

        total_bytes += len;


        if (status == SANE_STATUS_EOF)
        {
            break;
        }
        fwrite(buf8, 1, len, bunch.out);
        image.data[offset] = buf8;
        for (int i = 0; i < len; ++i)
        {
            image.data[offset + i] = buf8[i];
        }
        offset +=len;
        
    }
    printf("Done with the for loop\n");
   convert_to_jpeg();

    // jpeg_finish_compress(&cinfo);

    // fflush(ofp);

    // jpeg_destroy_compress(&cinfo);
    // free(jpegbuf);

    // if (image.data)
    // free(image.data);

    //-----------------------------------------------------------------------------------------------------

    fclose(bunch.out);

    sane_cancel(handle);
    sane_close(handle);

    sane_exit();
    printf("%s", "Finished executing");
    return 0;
}
