#include "capp.h"
#include "MAIN_MENU_1920x1080_PNG_bin.h"
#include "OPTIONS_MENU_1920x1080_PNG_bin.h"
#include "GAME_LIST_1920x1080_PNG_bin.h"
#include "ROM_INFO_1920x1080_PNG_bin.h"
#include "ZIP_INFO_1920x1080_PNG_bin.h"
#include "FILE_BROWSER_1920x1080_PNG_bin.h"

// =======================================================================
// TEXTURE / GRAPHICS  / RENDERING



void c_fbaRL::ScaleLine(u32 *Target, u32 *Source, u32 SrcWidth, u32 TgtWidth) {
 //Thanks to: http://www.compuphase.com/graphic/scale.htm
	int NumPixels1 = 0;
	u32 IntPart1 = 0;
	u32 FractPart1 = 0;
	NumPixels1 = TgtWidth;
	IntPart1 = SrcWidth / TgtWidth;
	FractPart1 = SrcWidth % TgtWidth;
	int E1 = 0;

	while (NumPixels1-- > 0) {
		*Target++ = *Source;
		Source += IntPart1;
		E1 += FractPart1;
		if ((u32)E1 >= TgtWidth) {
			E1 -= TgtWidth;
			Source++;
		} /* if */
	} /* while */
	return;
}





int c_fbaRL::ResizeImage(pngData *png_in, void *TgtTexture, u32 TgtWidth, u32 TgtHeight) {
 //Thanks to: http://www.compuphase.com/graphic/scale.htm
    if (TgtTexture == NULL)
        return 1;

    int NumPixels0 = 0;
    u32 IntPart0 = 0;
    u32 FractPart0 = 0;
    int E0 = 0;
	if(png_in->bmp_out){
		u32 *Source = (u32 *)(void *)png_in->bmp_out;
		u32 *Target = (u32 *)TgtTexture;

		NumPixels0 = TgtHeight;
		IntPart0 = (png_in->height / TgtHeight) * png_in->width;
		FractPart0 = png_in->height % TgtHeight;
		E0 = 0;
		u32 *PrevSource = NULL;

		while (NumPixels0-- > 0) {
			if (Source == PrevSource) {
				memcpy(Target, Target-TgtWidth, TgtWidth*sizeof(*Target));
			} else {
				ScaleLine(Target, Source, png_in->width, TgtWidth);
				PrevSource = Source;
			}
			Target += TgtWidth;
			Source += IntPart0;
			E0 += FractPart0;
			if ((u32)E0 >= TgtHeight) {
				E0 -= TgtHeight;
				Source += png_in->width;
			}

		}
        return 0;
	}
	return 1;
}

void c_tex::Render(u16 x, u16 y, u16 w, u16 h)
{
	if(!bTextureOK) return;

	if(w == 0 || h == 0)	{
		w = app.width;
		h = app.height;
	}

}


c_tex::c_tex(uint32_t _nTexture, uint32_t display)
{
    png = NULL;
    pngSec = NULL;
    png = new pngData;
    u32* texture;
    nTexture = _nTexture;
    //uint32_t i = display * 7 + nTexture;
    bTextureOK = false;
    u32 w, h, x, y;

    switch (nTexture) {
    case 0:
        pngLoadFromBuffer(MAIN_MENU_1920x1080_PNG_bin, MAIN_MENU_1920x1080_PNG_bin_size, png);
        break;
    case 1:
        pngLoadFromBuffer(GAME_LIST_1920x1080_PNG_bin, GAME_LIST_1920x1080_PNG_bin_size, png);
        break;
    case 2:
        pngLoadFromBuffer(ZIP_INFO_1920x1080_PNG_bin, ZIP_INFO_1920x1080_PNG_bin_size, png);
        break;
    case 3:
        pngLoadFromBuffer(ROM_INFO_1920x1080_PNG_bin, ROM_INFO_1920x1080_PNG_bin_size, png);
        break;
    case 4:
        pngLoadFromBuffer(OPTIONS_MENU_1920x1080_PNG_bin, OPTIONS_MENU_1920x1080_PNG_bin_size, png);
        break;
    case 5:
        pngLoadFromBuffer(FILE_BROWSER_1920x1080_PNG_bin, FILE_BROWSER_1920x1080_PNG_bin_size, png);
        break;
 
    }
    
    texture = (u32*)rsxMemalign(128, (png->height * png->pitch));

    if (!texture) return;
    memcpy((void*)texture, (void*)png->bmp_out, png->height * png->pitch);
    SAFE_FREE(png->bmp_out);
    png->bmp_out = texture;
    gcmAddressToOffset(texture, &pngBmpOffset);

    switch (nTexture) {
        case TEX_MAIN_MENU:
            nQuads = 9;
            quad[0] = createQuad(Point3(-1.0, 0.6, 0), Point3(0.1, 1.0, 0), Point3(-1.0, 1.0, 0), Point3(0.1, 0.6, 0));
            quad[1] = createQuad(Point3(0.1, 0.6, 0), Point3(1.0, 1.0, 0), Point3(0.1, 1.0, 0), Point3(1.0, 0.6, 0));
            quad[2] = createQuad(Point3(-1.0, -0.3, 0), Point3(-0.9, 0.6, 0), Point3(-1.0, 0.6, 0), Point3(-0.9, -0.3, 0));
            quad[3] = createQuad(Point3(-0.9, -0.3, 0), Point3(0.1, 0.6, 0), Point3(-0.9, 0.6, 0), Point3(0.1, -0.3, 0));
            quad[4] = createQuad(Point3(0.1, -0.3, 0), Point3(1.0, 0.6, 0), Point3(0.1, 0.6, 0), Point3(1.0, -0.3, 0));
            quad[5] = createQuad(Point3(-1.0, -1.0, 0), Point3(-0.9, -0.3, 0), Point3(-1.0, -0.3, 0), Point3(-0.9, -1.0, 0));
            quad[6] = createQuad(Point3(-0.9, -0.6, 0), Point3(0.1, -0.3, 0), Point3(-0.9, -0.3, 0), Point3(0.1, -0.6, 0));
            quad[7] = createQuad(Point3(-0.9, -1.0, 0), Point3(0.1, -0.6, 0), Point3(-0.9, -0.6, 0), Point3(0.1, -1.0, 0));
            quad[8] = createQuad(Point3(0.1, -1.0, 0), Point3(1.0, -0.3, 0), Point3(0.1, -0.3, 0), Point3(1.0, -1.0, 0));

            w = 1056; h = 216; x = 0; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[0] = texture;
            gcmAddressToOffset(texture, &texture_offset[0]);
            texture_dim[0].w = w;
            texture_dim[0].h = h;

           
            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
               texture_offset[0], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, 10, 4);
            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[0], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);
            
            w = 864; h = 216; x = 1056; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[1] = texture;
            gcmAddressToOffset(texture, &texture_offset[1]);
            texture_dim[1].w = w;
            texture_dim[1].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[1], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            w = 96; h = 486; x = 0; y = 216;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[2] = texture;
            gcmAddressToOffset(texture, &texture_offset[2]);
            texture_dim[2].w = w;
            texture_dim[2].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[2], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            w = 960; h = 486; x = 96; y = 216;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[3] = texture;
            gcmAddressToOffset(texture, &texture_offset[3]);
            texture_dim[3].w = w;
            texture_dim[3].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[3], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            w = 864; h = 486; x = 1056; y = 216;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[4] = texture;
            gcmAddressToOffset(texture, &texture_offset[4]);
            texture_dim[4].w = w;
            texture_dim[4].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[4], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            w = 96; h = 378; x = 0; y = 702;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[5] = texture;
            gcmAddressToOffset(texture, &texture_offset[5]);
            texture_dim[5].w = w;
            texture_dim[5].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[5], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            w = 960; h = 162; x = 96; y = 702;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[6] = texture;
            gcmAddressToOffset(texture, &texture_offset[6]);
            texture_dim[6].w = w;
            texture_dim[6].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[6], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            w = 960; h = 216; x = 96; y = 864;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[7] = texture;
            gcmAddressToOffset(texture, &texture_offset[7]);
            texture_dim[7].w = w;
            texture_dim[7].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[7], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            w = 864; h = 378; x = 1056; y = 702;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[8] = texture;
            gcmAddressToOffset(texture, &texture_offset[8]);
            texture_dim[8].w = w;
            texture_dim[8].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[8], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            break;

        case TEX_GAME_LIST:
            nQuads = 6;
            
            quad[0] = createQuad(Point3(-0.9802, -0.6, 0), Point3(-0.4063, 0.2519, 0), Point3(-0.9802, 0.2519, 0), Point3(-0.4063, -0.6, 0));
            quad[1] = createQuad(Point3(-0.9802, 0.3, 0), Point3(-0.4063, 0.9630, 0), Point3(-0.9802, 0.9630, 0), Point3(-0.4063, 0.3, 0));
            quad[2] = createQuad(Point3(-1.0, 0.0, 0), Point3(0.0, 1.0, 0), Point3(-1.0, 1.0, 0), Point3(0.0, 0.0, 0));
            quad[3] = createQuad(Point3(0.0, 0.0, 0), Point3(1.0, 1.0, 0), Point3(0.0, 1.0, 0), Point3(1.0, 0.0, 0));
            quad[4] = createQuad(Point3(-1.0, -1.0, 0), Point3(0.0, 0.0, 0), Point3(-1.0, 0.0, 0), Point3(0.0, -1.0, 0));
            quad[5] = createQuad(Point3(0.0, -1.0, 0), Point3(1.0, 0.0, 0),  Point3(0.0, 0.0, 0), Point3(1.0, -1.0, 0));
            
            w = 960; h = 540; x = 0; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[2] = texture;
            gcmAddressToOffset(texture, &texture_offset[2]);
            texture_dim[2].w = w;
            texture_dim[2].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[2], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            w = 960; h = 540; x = 960; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[3] = texture;
            gcmAddressToOffset(texture, &texture_offset[3]);
            texture_dim[3].w = w;
            texture_dim[3].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[3], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            w = 960; h = 540; x = 0; y = 540;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[4] = texture;
            gcmAddressToOffset(texture, &texture_offset[4]);
            texture_dim[4].w = w;
            texture_dim[4].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[4], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            w = 960; h = 540; x = 960; y = 540;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[5] = texture;
            gcmAddressToOffset(texture, &texture_offset[5]);
            texture_dim[5].w = w;
            texture_dim[5].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[5], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            break;
 
        case TEX_OPTIONS:
        case TEX_FILEBROWSER:
        case TEX_ZIP_INFO:
            nQuads = 4;

            quad[0] = createQuad(Point3(-1.0, 0.0, 0), Point3(0.0, 1.0, 0), Point3(-1.0, 1.0, 0), Point3(0.0, 0.0, 0));
            quad[1] = createQuad(Point3(0.0, 0.0, 0), Point3(1.0, 1.0, 0), Point3(0.0, 1.0, 0), Point3(1.0, 0.0, 0));
            quad[2] = createQuad(Point3(-1.0, -1.0, 0), Point3(0.0, 0.0, 0), Point3(-1.0, 0.0, 0), Point3(0.0, -1.0, 0));
            quad[3] = createQuad(Point3(0.0, -1.0, 0), Point3(1.0, 0.0, 0), Point3(0.0, 0.0, 0), Point3(1.0, -1.0, 0));


            w = MAX_WIDTH/2; h = MAX_HEIGHT/2; x = 0; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[0] = texture;
            gcmAddressToOffset(texture, &texture_offset[0]);
            texture_dim[0].w = w;
            texture_dim[0].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[0], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            x = MAX_WIDTH / 2; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[1] = texture;
            gcmAddressToOffset(texture, &texture_offset[1]);
            texture_dim[1].w = w;
            texture_dim[1].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[1], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            x = 0; y = MAX_HEIGHT / 2;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[2] = texture;
            gcmAddressToOffset(texture, &texture_offset[2]);
            texture_dim[2].w = w;
            texture_dim[2].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[2], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            x = MAX_WIDTH / 2; y = MAX_HEIGHT / 2;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[3] = texture;
            gcmAddressToOffset(texture, &texture_offset[3]);
            texture_dim[3].w = w;
            texture_dim[3].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[3], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            break;

        case TEX_ROM_INFO:
           
            nQuads = 5;
            quad[0] = createQuad(Point3(0.3219, -0.0148, 0), Point3(0.8958, 0.6519, 0), Point3(0.3219, 0.6519, 0), Point3(0.8958, -0.0148, 0));
            quad[1] = createQuad(Point3(-1.0, 0.0, 0), Point3(0.0, 1.0, 0), Point3(-1.0, 1.0, 0), Point3(0.0, 0.0, 0));
            quad[2] = createQuad(Point3(0.0, 0.0, 0), Point3(1.0, 1.0, 0), Point3(0.0, 1.0, 0), Point3(1.0, 0.0, 0));
            quad[3] = createQuad(Point3(-1.0, -1.0, 0), Point3(0.0, 0.0, 0), Point3(-1.0, 0.0, 0), Point3(0.0, -1.0, 0));
            quad[4] = createQuad(Point3(0.0, -1.0, 0), Point3(1.0, 0.0, 0), Point3(0.0, 0.0, 0), Point3(1.0, -1.0, 0));


            w = MAX_WIDTH / 2; h = MAX_HEIGHT / 2; x = 0; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[1] = texture;
            gcmAddressToOffset(texture, &texture_offset[1]);
            texture_dim[1].w = w;
            texture_dim[1].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[1], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            x = MAX_WIDTH / 2; y = 0;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[2] = texture;
            gcmAddressToOffset(texture, &texture_offset[2]);
            texture_dim[2].w = w;
            texture_dim[2].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[2], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            x = 0; y = MAX_HEIGHT / 2;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[3] = texture;
            gcmAddressToOffset(texture, &texture_offset[3]);
            texture_dim[3].w = w;
            texture_dim[3].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[3], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);


            x = MAX_WIDTH / 2; y = MAX_HEIGHT / 2;
            texture = (u32*)rsxMemalign(128, w * h * 4);
            if (!texture) exit(0);
            texture_buffer[4] = texture;
            gcmAddressToOffset(texture, &texture_offset[4]);
            texture_dim[4].w = w;
            texture_dim[4].h = h;

            rsxSetTransferImage(gGcmContext, GCM_TRANSFER_LOCAL_TO_LOCAL,
                texture_offset[4], w * 4, 0, 0, pngBmpOffset + (x + y * MAX_WIDTH) * 4, PITCH, 0, 0, w, h, 4);

            break;

    }

    
    bTextureOK = true;



}

int c_tex::BindTexture(char* pszPath)
{
    bTextureOK = false;
    //pngData *pngTmp = new pngData;
    u32* texture;

    if (fileExist(pszPath))
    {
        if (png != NULL) {
            if (png->bmp_out != NULL) {
                SAFE_FREE(png->bmp_out);
            }
            SAFE_DELETE(png)
        }
        png = new pngData;
        

        pngLoadFromFile(pszPath, png);
        if (png->pitch <= 0 ) {
            printf("Load png error\n");
            return 0;
        }
        if (app.textures[TEX_GAME_LIST]->texture_buffer[0])
        {
            rsxFree(app.textures[TEX_GAME_LIST]->texture_buffer[0]);
            app.textures[TEX_GAME_LIST]->texture_buffer[0] = NULL;
        }
        
        //adjust aspect ratio of preview image
        if (png->width < png->height) {
            app.textures[TEX_GAME_LIST]->quad[0]->vertices[0].pos = Vector3(-0.9802 + ((553.0 - (462.0 * png->width / png->height)) / 1920), -0.6, 0.0);
            app.textures[TEX_GAME_LIST]->quad[0]->vertices[1].pos = Vector3(-0.4063 - ((553.0 - (462.0 * png->width / png->height)) / 1920), 0.2519, 0.0);
            app.textures[TEX_GAME_LIST]->quad[0]->vertices[2].pos = Vector3(-0.9802 + ((553.0 - (462.0 * png->width / png->height)) / 1920), 0.2519, 0.0);
            app.textures[TEX_GAME_LIST]->quad[0]->vertices[3].pos = Vector3(-0.4063 - ((553.0 - (462.0 * png->width / png->height)) / 1920), -0.6, 0.0);
        }
        else
        {
            if ((float)553.0 * ((float)png->height / (float)png->width) > 462.0f) {
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[0].pos = Vector3(-0.9802, -0.600, 0.0);
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[1].pos = Vector3(-0.4063, 0.2519, 0.0);
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[2].pos = Vector3(-0.9802, 0.2519, 0.0);
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[3].pos = Vector3(-0.4063, -0.600, 0.0);
            }
            else {
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[0].pos = Vector3(-0.9802, -0.600 + ((462.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[1].pos = Vector3(-0.4063, 0.2519 - ((462.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[2].pos = Vector3(-0.9802, 0.2519 - ((462.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_GAME_LIST]->quad[0]->vertices[3].pos = Vector3(-0.4063, -0.600 + ((462.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
            }
        }
        texture = (u32*)rsxMemalign(128, png->width * png->height * 4);
        if (!texture) exit(0);
        app.textures[TEX_GAME_LIST]->texture_buffer[0] = texture;
        gcmAddressToOffset(texture, &(app.textures[TEX_GAME_LIST]->texture_offset[0]));
        app.textures[TEX_GAME_LIST]->texture_dim[0].w = png->width;
        app.textures[TEX_GAME_LIST]->texture_dim[0].h = png->height;
        memcpy(app.textures[TEX_GAME_LIST]->texture_buffer[0], png->bmp_out, png->pitch * png->height);
       
        return 1;
        /*u32 out_w, out_h;
        if (pngTmp->width < pngTmp->height) {
            out_h = 460.0f ;
            out_w = round((float)out_h * ((float)pngTmp->width / (float)pngTmp->height));

        }
        else {
            out_w = 551.0f ;
            out_h = round((float)out_w * ((float)pngTmp->height / (float)pngTmp->width));
            if (out_h > 460.0f) {
                out_h = 460.0f;
                out_w = round((float)out_h * ((float)pngTmp->width / (float)pngTmp->height));
            }
        }


        png->bmp_out = malloc(out_w * out_h * sizeof(u32));

        if (png->bmp_out == NULL) {
            printf("Memory error png temp\n");
            exit(0);
        }

        int i = 0;

        i = fbaRL->ResizeImage(pngTmp, png->bmp_out, out_w, out_h);
        SAFE_FREE(pngTmp->bmp_out);
        SAFE_DELETE(pngTmp);

        if (i == 0) {

            png->width = out_w;
            png->pitch = out_w * sizeof(uint32_t);
            png->height = out_h;
            bTextureOK = true;
            return 1;

        }
        else {
            printf("Memory error png resize\n");
            return 0;

        }*/
 
    }
 
    return 0;
}

int c_tex::BindSecTexture(char* pszPath)
{
	//pngData *pngTmp = new pngData;
	bTextureOK = false;
    u32* texture;
	//pngSec = new pngData;

    if (fileExist(pszPath))
    {

        if (pngSec != NULL) {
            if(pngSec->bmp_out != NULL) {
                //printf("pngSec pieno\n");
                SAFE_FREE(pngSec->bmp_out);
                }

        SAFE_DELETE(pngSec)
        }
        pngSec = new pngData;

        pngLoadFromFile(pszPath, pngSec);
        if (pngSec->pitch <= 0) {
            printf("Load pngSec error\n");
            return 0;
        }
        if (app.textures[TEX_GAME_LIST]->texture_buffer[1])
        {
            rsxFree(app.textures[TEX_GAME_LIST]->texture_buffer[1]);
            app.textures[TEX_GAME_LIST]->texture_buffer[1] = NULL;
        }

        if (app.textures[TEX_ROM_INFO]->texture_buffer[0])
        {
            rsxFree(app.textures[TEX_ROM_INFO]->texture_buffer[0]);
            app.textures[TEX_ROM_INFO]->texture_buffer[0] = NULL;
        }

        //adjust aspect ratio of title image
        //if (pngSec->width < pngSec->height) {
        //    //quad[1] = createQuad(Point3(-0.9802, 0.3, 0), Point3(-0.4063, 0.9630, 0), Point3(-0.9802, 0.9630, 0), Point3(-0.4063, 0.3, 0));
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[0].pos = Vector3(-0.9802 + ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.3, 0.0);
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[1].pos = Vector3(-0.4063 - ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.9630, 0.0);
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[2].pos = Vector3(-0.9802 + ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.9630, 0.0);
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[3].pos = Vector3(-0.4063 - ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.3, 0.0);
        //}
        //else
        //{
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[0].pos = Vector3(-0.9802, 0.3000 + ((360.0 - (553.0 * pngSec->height / pngSec->width)) / 1080), 0.0);
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[1].pos = Vector3(-0.4063, 0.9630 - ((360.0 - (553.0 * pngSec->height / pngSec->width)) / 1080), 0.0);
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[2].pos = Vector3(-0.9802, 0.9630 - ((360.0 - (553.0 * pngSec->height / pngSec->width)) / 1080), 0.0);
        //    app.textures[TEX_GAME_LIST]->quad[1]->vertices[3].pos = Vector3(-0.4063, 0.3000 + ((360.0 - (553.0 * pngSec->height / pngSec->width)) / 1080), 0.0);
        //}

        //adjust aspect ratio of title image
        if (png->width < png->height) {
            app.textures[TEX_GAME_LIST]->quad[1]->vertices[0].pos = Vector3(-0.9802 + ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.3000, 0.0);
            app.textures[TEX_GAME_LIST]->quad[1]->vertices[1].pos = Vector3(-0.4063 - ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.9630, 0.0);
            app.textures[TEX_GAME_LIST]->quad[1]->vertices[2].pos = Vector3(-0.9802 + ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.9630, 0.0);
            app.textures[TEX_GAME_LIST]->quad[1]->vertices[3].pos = Vector3(-0.4063 - ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.3000, 0.0);
            app.textures[TEX_ROM_INFO]->quad[0]->vertices[0].pos = Vector3(0.3219 + ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), -0.6519, 0.0);
            app.textures[TEX_ROM_INFO]->quad[0]->vertices[1].pos = Vector3(0.8958 - ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.0148, 0.0);
            app.textures[TEX_ROM_INFO]->quad[0]->vertices[2].pos = Vector3(0.3219 + ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), 0.0148, 0.0);
            app.textures[TEX_ROM_INFO]->quad[0]->vertices[3].pos = Vector3(0.8958 - ((553.0 - (360.0 * pngSec->width / pngSec->height)) / 1920), -0.6519, 0.0);

        }
        else
        {
            if ((float)553.0 * ((float)png->height / (float)png->width) > 360.0f) {
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[0].pos = Vector3(-0.9802, 0.3000, 0.0);
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[1].pos = Vector3(-0.4063, 0.9630, 0.0);
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[2].pos = Vector3(-0.9802, 0.9630, 0.0);
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[3].pos = Vector3(-0.4063, 0.3000, 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[0].pos = Vector3(0.3219, -0.6519, 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[1].pos = Vector3(0.8958, 0.0148, 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[2].pos = Vector3(0.3219, 0.0148, 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[3].pos = Vector3(0.8958, -0.6519, 0.0);
            }
            else {
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[0].pos = Vector3(-0.9802, 0.3000 + ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[1].pos = Vector3(-0.4063, 0.9630 - ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[2].pos = Vector3(-0.9802, 0.9630 - ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_GAME_LIST]->quad[1]->vertices[3].pos = Vector3(-0.4063, 0.3000 + ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[0].pos = Vector3(0.3219, -0.6519 + ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[1].pos = Vector3(0.8958, 0.0148 - ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[2].pos = Vector3(0.3219, 0.0148 - ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
                app.textures[TEX_ROM_INFO]->quad[0]->vertices[3].pos = Vector3(0.8958, -0.6519 + ((360.0 - (553.0 * png->height / png->width)) / 1080), 0.0);
            }
        }

        texture = (u32*)rsxMemalign(128, pngSec->width * pngSec->height * 4);
        if (!texture) exit(0);
        app.textures[TEX_GAME_LIST]->texture_buffer[1] = texture;
        gcmAddressToOffset(texture, &(app.textures[TEX_GAME_LIST]->texture_offset[1]));
        app.textures[TEX_GAME_LIST]->texture_dim[1].w = pngSec->width;
        app.textures[TEX_GAME_LIST]->texture_dim[1].h = pngSec->height;
        memcpy(app.textures[TEX_GAME_LIST]->texture_buffer[1], pngSec->bmp_out, pngSec->pitch * pngSec->height);
        app.textures[TEX_ROM_INFO]->texture_buffer[0] = texture;
        gcmAddressToOffset(texture, &(app.textures[TEX_ROM_INFO]->texture_offset[0]));
        app.textures[TEX_ROM_INFO]->texture_dim[0].w = pngSec->width;
        app.textures[TEX_ROM_INFO]->texture_dim[0].h = pngSec->height;
        memcpy(app.textures[TEX_ROM_INFO]->texture_buffer[0], pngSec->bmp_out, pngSec->pitch * pngSec->height);

        return 1;
        /*if(pngTmp->width <= 0)
		{
			return 0;
		} else {
			u32 out_w, out_h;
			if (pngTmp->width < pngTmp->height) {
                out_h = 360.0f;
                out_w = round((float)out_h * ((float)pngTmp->width / (float)pngTmp->height));

            }
            else {
                out_w = 551.0f;
                out_h = round((float)out_w * ((float)pngTmp->height / (float)pngTmp->width));
                if (out_h > 360.0f) {
                    out_h = 360.0f;
                    out_w = round((float)out_h * ((float)pngTmp->width / (float)pngTmp->height));
                }
            }


            pngSec->bmp_out = malloc(out_w * out_h * sizeof(u32));
            if (pngSec->bmp_out == NULL) {
                printf("Memory error pngSec temp\n");
                exit(0);
            }

            int i = 0;

            i =  fbaRL->ResizeImage(pngTmp, pngSec->bmp_out, out_w, out_h);

            SAFE_FREE(pngTmp->bmp_out);
            SAFE_DELETE(pngTmp);

            if (i == 0) {

                    pngSec->width = out_w;
                    pngSec->pitch = out_w * sizeof(uint32_t);
                    pngSec->height = out_h;
                    bTextureOK = true;
                    return 1;

            }
            else {
                printf("Memory error png resize\n");
                return 0;

            }*/


	}
	
    return 0;
}


c_tex::c_tex(uint32_t _nTexture, char* szPath)
{
	png = NULL;
	pngSec = NULL;

	//Min = new Minimum;
	//IMG = new Image(Min);
	png = new pngData;
	png->bmp_out = NULL;
    png->width = 0; 
    png->height = 0;
	pngSec = new pngData;
	pngSec->bmp_out = NULL;
    pngSec->width = 0;
    pngSec->height = 0;

	nTexture = _nTexture;

    
    //	pszTexPath = (char*)malloc(2048);
    //	memset(pszTexPath, 0, 2048);

    //	strcpy(pszTexPath, szPath);

	    if(!BindTexture(szPath))
		    printf("Error: can't BinTexture: %s\n", szPath);

        if (_nTexture == TEX_PREVIEW) { // preview has to be resized
                //pngData *png_out = NULL;
    //            printf("(Screen (%dx%d) - pre resize %d - %d - %p\n", app.width, app.height, png->width, png->height, png->bmp_out);


            if (!BindSecTexture((char*)"/dev_hdd0/game/FBNE00123/USRDIR/cores/borders/default/TITLES.PNG"))
                printf("Error: can't BinSecTexture: %s\n", szPath);
        }
        //printf("End texture %s\n", szPath);
        
}


c_tex::~c_tex()
{
	printf("Destroy c_tex %d\n", nTexture);
	if(bTextureOK)
	{
		////glDeleteTextures(1, &nTexId[0]);
		//*&nTexId[0] = NULL;
		//*&bTextureOK = NULL;
	}
	////DA rivedere!!!!
//	printf("pszTexPath c_tex\n");
//	if (pszTexPath != NULL)
//        SAFE_FREE(pszTexPath);
//	printf("IMG c_tex\n");
//	if (IMG != NULL)
//        delete (IMG);
//	printf("Min c_tex\n");
//    if (Min != NULL)
//        delete(Min);

	if (png->bmp_out != NULL) {
//        printf("%d - %d - %p\n", png->width, png->height, png->bmp_out);
        SAFE_FREE(png->bmp_out);
	}


    //printf("png c_tex\n");
	SAFE_DELETE (png);
	if (pngSec != NULL) {
        if (pngSec->bmp_out != NULL) {
//        printf("%d - %d - %p\n", png->width, png->height, png->bmp_out);
        SAFE_FREE(pngSec->bmp_out);
        }

	}



    SAFE_DELETE(pngSec);
//	printf("Exit\n");
}


void c_fbaRL::ResetPreviewImage()
{
    app.textures[TEX_PREVIEW]->BindTexture(g_opt_szTextures[TEX_PREVIEW]);
	app.textures[TEX_PREVIEW]->BindSecTexture(g_opt_szTextures[TEX_PREVIEW_TITLES]);
    //printf("Exit ResetPreviewImage\n");
}

void c_fbaRL::UpdatePreviewImage()
{
	if(nSection == SECTION_GAMELIST || nSection == SECTION_MAIN)
	{
		//printf("UpdatePreviewImage enter SECTION_GAMELIST %s\n", g_opt_szTextures[TEX_PREVIEW]);
		char szPreviewPath[512] = { 0 };

		if (strcmp(games[nBurnSelected]-> sysmask, "MASKSNES") == 0) {
                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/SNES/%s.png",games[nBurnSelected]->name);
		}
		else
            if (strcmp(games[nBurnSelected]->subsystem, "megacd") == 0) {
                  sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/MEGACD/%s.png",games[nBurnSelected]->name);
             }
             else
                if (strcmp(games[nBurnSelected]->sysmask, "MASKMEGADRIVE") == 0) {
                    sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/MEGADRIVE/%s.png", games[nBurnSelected]->name);
                }
                else
                    if (strcmp(games[nBurnSelected]->sysmask, "MASKAMIGA") == 0) {
                         sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/AMIGA/%s.png",games[nBurnSelected]->name);
                     }
                     else
                         if (strcmp(games[nBurnSelected]->sysmask, "MASKCOLECO") == 0) {
                             sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/COLECO/%s.png", games[nBurnSelected]->name);
                         }
                         else
                             if (strcmp(games[nBurnSelected]->sysmask, "MASKTG16") == 0) {
                                 sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/TG16/%s.png", games[nBurnSelected]->name);
                             }
                             else
                                 if (strcmp(games[nBurnSelected]->sysmask, "MASKPCE") == 0) {
                                     sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/PCE/%s.png", games[nBurnSelected]->name);
                                 }
                                 else
                                     if (strcmp(games[nBurnSelected]->sysmask, "MASKSGX") == 0) {
                                         sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/SGX/%s.png", games[nBurnSelected]->name);
                                     }
                                     else
                                         sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/%s.png", games[nBurnSelected]->name);
        //printf("szPreviewPath: %s\n", szPreviewPath);
		// try main romset name...
		if(fileExist(szPreviewPath)) {
            app.textures[TEX_PREVIEW]->BindTexture(szPreviewPath);
		}
        else
		// try parent...
		{
			// clean memory again...
			////delete app.textures[TEX_PREVIEW];
			////*&app.textures[TEX_PREVIEW] = NULL;

			if (games[nBurnSelected]->isClone == false)
                    //app.textures[TEX_PREVIEW] = new c_tex(TEX_PREVIEW, g_opt_szTextures[TEX_PREVIEW]);
                    app.textures[TEX_PREVIEW]->BindTexture(g_opt_szTextures[TEX_PREVIEW]);
            else {
//                uint32_t parent = games[fgames[nSelectedGame]->GameID]->parent_id;
                if (strcmp(games[nBurnSelected]->sysmask, "MASKSNES") == 0) {
                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/SNES/%s.png",games[nBurnSelected]->parent_name);
                }
                else
                    if (strcmp(games[nBurnSelected]->subsystem, "megacd") == 0) {
                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/MEGACD/%s.png", games[nBurnSelected]->parent_name);
                    }
                    else
                        if (strcmp(games[nBurnSelected]->sysmask, "MASKMEGADRIVE") == 0) {
                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/MEGADRIVE/%s.png",games[nBurnSelected]->parent_name);
                        }
                        else
                            if (strcmp(games[nBurnSelected]->sysmask, "MASKAMIGA") == 0) {
                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/AMIGA/%s.png",games[nBurnSelected]->parent_name);
                            }
                            else
                                if (strcmp(games[nBurnSelected]->sysmask, "MASKCOLECO") == 0) {
                                    sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/COLECO/%s.png", games[nBurnSelected]->parent_name);
                                }
                                else
                                    if (strcmp(games[nBurnSelected]->sysmask, "MASKTG16") == 0) {
                                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/TG16/%s.png", games[nBurnSelected]->parent_name);
                                    }
                                    else
                                        if (strcmp(games[nBurnSelected]->sysmask, "MASKPCE") == 0) {
                                            sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/PCE/%s.png", games[nBurnSelected]->parent_name);
                                        }
                                        else
                                            if (strcmp(games[nBurnSelected]->sysmask, "MASKSGX") == 0) {
                                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/SGX/%s.png", games[nBurnSelected]->parent_name);
                                            }
                                            else
                                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/previews/%s.png", games[nBurnSelected]->parent_name);



                if(fileExist(szPreviewPath))
                        //app.textures[TEX_PREVIEW] = new c_tex(TEX_PREVIEW, szPreviewPath);
                        app.textures[TEX_PREVIEW]->BindTexture(szPreviewPath);
                //app.textures[TEX_PREVIEW]->BindTexture(szPreviewPath);
                else
                    // no preview available...
                    //ResetPreviewImage();
                    //app.textures[TEX_PREVIEW] = new c_tex(TEX_PREVIEW, g_opt_szTextures[TEX_PREVIEW]);
                    app.textures[TEX_PREVIEW]->BindTexture(g_opt_szTextures[TEX_PREVIEW]);
            }
		}

		//Boxart
		//printf("Update title\n");
		if (strcmp(games[nBurnSelected]->sysmask, "MASKSNES") == 0) {
                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/SNES/%s.png",games[nBurnSelected]->name);
		}
		else
            if (strcmp(games[nBurnSelected]->subsystem, "megacd") == 0) {
                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/MEGACD/%s.png", games[nBurnSelected]->name);
            }
            else if (strcmp(games[nBurnSelected]->sysmask, "MASKMEGADRIVE") == 0) {
                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/MEGADRIVE/%s.png",games[nBurnSelected]->name);
                }
                else
                    if (strcmp(games[nBurnSelected]->sysmask, "MASKAMIGA") == 0) {
                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/AMIGA/%s.png",games[nBurnSelected]->name);
                    }
                    else
                        if (strcmp(games[nBurnSelected]->sysmask, "MASKCOLECO") == 0) {
                            sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/COLECO/%s.png", games[nBurnSelected]->name);
                        }
                        else
                            if (strcmp(games[nBurnSelected]->sysmask, "MASKTG16") == 0) {
                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/TG16/%s.png", games[nBurnSelected]->name);
                            }
                            else
                                if (strcmp(games[nBurnSelected]->sysmask, "MASKPCE") == 0) {
                                    sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/PCE/%s.png", games[nBurnSelected]->name);
                                }
                                else
                                    if (strcmp(games[nBurnSelected]->sysmask, "MASKSGX") == 0) {
                                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/SGX/%s.png", games[nBurnSelected]->name);
                                    }
                                    else
                                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/%s.png", games[nBurnSelected]->name);



		// try main romset name...
		if(fileExist(szPreviewPath)) {
          //printf("Pre title %s\n", szPreviewPath);
            app.textures[TEX_PREVIEW]->BindSecTexture(szPreviewPath);
            //printf("Post title\n");
		}
        else {

			if (games[nBurnSelected]->isClone == false)
                    app.textures[TEX_PREVIEW]->BindSecTexture(g_opt_szTextures[TEX_PREVIEW_TITLES]);
            else {
//                uint32_t parent = games[fgames[nSelectedGame]->GameID]->parent_id;

                if (strcmp(games[nBurnSelected]->sysmask, "MASKSNES") == 0) {
                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/SNES/%s.png",games[nBurnSelected]->parent_name);
                }
                else
                    if (strcmp(games[nBurnSelected]->subsystem, "megacd") == 0) {
                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/MEGACD/%s.png", games[nBurnSelected]->parent_name);
                    }
                    else
                        if (strcmp(games[nBurnSelected]->sysmask, "MASKMEGADRIVE") == 0) {
                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/MEGADRIVE/%s.png",games[nBurnSelected]->parent_name);
                        }
                        else
                            if (strcmp(games[nBurnSelected]->sysmask, "MASKAMIGA") == 0) {
                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/AMIGA/%s.png",games[nBurnSelected]->parent_name);
                            }
                            else
                                if (strcmp(games[nBurnSelected]->sysmask, "MASKCOLECO") == 0) {
                                    sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/COLECO/%s.png", games[nBurnSelected]->parent_name);
                                }
                                else
                                    if (strcmp(games[nBurnSelected]->sysmask, "MASKTG16") == 0) {
                                        sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/TG16/%s.png", games[nBurnSelected]->parent_name);
                                    }
                                    else
                                        if (strcmp(games[nBurnSelected]->sysmask, "MASKPCE") == 0) {
                                            sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/PCE/%s.png", games[nBurnSelected]->parent_name);
                                        }
                                        else
                                            if (strcmp(games[nBurnSelected]->sysmask, "MASKSGX") == 0) {
                                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/SGX/%s.png", games[nBurnSelected]->parent_name);
                                            }
                                            else
                                                sprintf(szPreviewPath, "/dev_hdd0/game/FBNE00123/USRDIR/cores/titles/%s.png", games[nBurnSelected]->parent_name);

                if(fileExist(szPreviewPath))
                        app.textures[TEX_PREVIEW]->BindSecTexture(szPreviewPath);
                //app.textures[TEX_PREVIEW]->BindTexture(szPreviewPath);
                else
                    // no preview available...
                    //ResetPreviewImage();
                    app.textures[TEX_PREVIEW]->BindSecTexture(g_opt_szTextures[TEX_PREVIEW_TITLES]);
            }
		}
		//End Boxart

	}
}
