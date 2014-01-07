#include "ASCBmpHelpers.h"
#include "ASCStreams.h"

ASCBoolean BmpLoadFromPNG(const wstring& sFileName, CASCBitmap* pBmp)
{
	if (!pBmp) 
		return false;

	FILE *fp;
	png_bytep* row_pointers = NULL;

	// �� PNG �ļ�
	if ((fp = _wfopen(sFileName.c_str(), L"rb")) == NULL)
	{
		// assert("File not exsist or open faild!");
		return false;
	}

	const ASCInt PNG_BYTES_TO_CHECK = 8;
	png_byte buf[PNG_BYTES_TO_CHECK];

	if (fread(buf, 1, PNG_BYTES_TO_CHECK, fp) != PNG_BYTES_TO_CHECK)
	{
		// assert("Png format error!");
		fclose(fp);
		return false;
	}

	// ����Ƿ��� PNG �ļ�
	if (png_sig_cmp(buf, (png_size_t)0, PNG_BYTES_TO_CHECK))
	{
		// assert("Png format error!");
		fclose(fp);
		return false;
	}

	// ���� PNG �ṹ
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		// assert("png_create_read_struct failed!");
		fclose(fp);
		return false;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr)
	{
		// assert("png_create_info_struct failed!");
		fclose(fp);
		return false;
	}

	// ��������
	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		if (row_pointers) 
			delete [] row_pointers;
		fclose(fp);
		return false;
	}

	// ���ñ�׼ IO
	png_init_io(png_ptr, fp);

	// ���� PNG �Ѿ������ļ�ͷ
	png_set_sig_bytes(png_ptr, PNG_BYTES_TO_CHECK);

	// �� PNG ��Ϣ
	png_read_info(png_ptr, info_ptr);

	// ��ͼ����Ϣ
	png_uint_32 width;      // ��
	png_uint_32 height;     // ��
	ASCInt bit_depth;       // ÿ����ɫͨ����λ��
	ASCInt color_type;      // ��ɫ����
	{
		png_uint_32 w,h;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
		width=w;
		height=h;
	}

	// ��һϵ����ɫת��

	// Convert palette color to true color
	if (color_type==PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	// Convert low bit colors to 8 bit colors
	if (bit_depth < 8)
	{
		if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		else
			png_set_packing(png_ptr);
	}

	// Expand paletted or RGB images with transparency to full alpha channels
	// so the data will be available as RGBA quartets.
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	// Convert high bit colors to 8 bit colors
	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	// Convert gray color to true color
	if (color_type==PNG_COLOR_TYPE_GRAY || color_type==PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	// Update the changes
	png_read_update_info(png_ptr, info_ptr);
	{
		png_uint_32 w,h;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
		width = w;
		height = h;
	}

	// Convert RGBA to BGRA
	if (color_type & PNG_COLOR_MASK_COLOR)
	{
		png_set_bgr(png_ptr);
	}

	// Update the changes
	{
		png_uint_32 w, h;
		png_get_IHDR(png_ptr, info_ptr, &w, &h, &bit_depth, &color_type, NULL, NULL, NULL);
		width = w;
		height = h;
	}

	// ����λͼ��Ϣ
	CASCPixelFormat ColorFormat;
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
		ColorFormat = apfA8R8G8B8;
	else
		ColorFormat = apfR8G8B8;
	pBmp->SetBmpInfo(width, height, ColorFormat);

	// ��ʼ��������
	row_pointers = new png_bytep[height];
	for (int i = 0; i < pBmp->GetHeight(); ++i)
		row_pointers[i] = (png_bytep)pBmp->GetLineBits(i);

	// ��ͼ������
	png_read_image(png_ptr, row_pointers);

	// ���
	png_read_end(png_ptr, NULL);
	png_destroy_read_struct(&png_ptr,&info_ptr, 0);
	fclose(fp);
	delete row_pointers;
	return true;
}

ASCBoolean BmpLoadFromTGA(const wstring& sFileName, CASCBitmap* pBmp)
{
	if (!pBmp)
		return false;

	CASCFileStream fs;
	if (!fs.Open(sFileName, GENERIC_READ))
		return false;

	CASCTGAHeader th;
	if (!fs.ReadBuffer(&th, sizeof(th)))
	{
		// assert("TGA format error!");
		return false;
	}

	// ����֧�� TGA
	if (((th.m_nImgType != TGA_IMGTYPE_TRUECOLOR) && (th.m_nImgType != TGA_IMGTYPE_RLETRUECOLOR))
		|| (th.m_nColorMapType != 0) 
		|| ((th.m_nBPP != 24) && (th.m_nBPP != 32))
		|| (((th.m_nImgDesc & 0x30) != 0x00) && ((th.m_nImgDesc & 0x30) != 0x20)))
	{
		// assert("TGA format no support!");
		return false;
	}

	// ���� ImageID ����
	if (th.m_nIDLen > 0)
		fs.Seek(th.m_nIDLen, asoCurrent);

	// ��ͼ������
	ASCInt nBytePP = th.m_nBPP >> 3;
	ASCInt nImgSize = th.m_nWidth * th.m_nHeight * nBytePP;
	ASCByte* pTgaBits = (ASCByte*)malloc(nImgSize);

	if (th.m_nImgType == TGA_IMGTYPE_TRUECOLOR)
	{
		if (!fs.ReadBuffer(pTgaBits, nImgSize))
		{
			// assert("TGA format error!");
			return false;
		}
	}
	else 
	{
		// RLE ��ѹ
		ASCByte* pBits = pTgaBits;
		ASCByte rh, blen;
		ASCInt rsize;
		ASCUInt32 rpx;
		ASCBoolean bSucc = true;

		while (nImgSize > 0)
		{
			if (!fs.ReadBuffer(&rh, sizeof(rh)))
			{
				bSucc = false;
				break;
			}

			blen =  (rh & 0x7F) + 1;
			if ((rh & 0x80) == 0x80)
			{
				if (!fs.ReadBuffer(&rpx, nBytePP))
				{
					bSucc = false;
					break;
				}
				while (blen > 0)
				{
					memcpy(pBits, &rpx, nBytePP);
					pBits += nBytePP;
					nImgSize -= nBytePP;
					--blen;
				}
			}
			else
			{
				rsize = blen * nBytePP;
				if (!fs.ReadBuffer(pBits, rsize))
				{
					bSucc = false;
					break;
				}
				pBits += rsize;
				nImgSize -= rsize;
			}
		}

		if (!bSucc)
		{
			free(pTgaBits);
			// assert("TGA format error!");
			return false;
		}
	}

	// ת�� BMP
	CASCPixelFormat format = apfR8G8B8;
	if (th.m_nBPP == 32)
	{
		format = apfA8R8G8B8;
	}

	ASCInt nHeight = ((th.m_nImgDesc & 0x30) == 0x00) ? th.m_nHeight : -th.m_nHeight;
	pBmp->SetBmpInfo(th.m_nWidth, nHeight, format);

	ASCByte* pSrcBits = pTgaBits;
	ASCByte* pDestBits = pBmp->GetBits();
	ASCInt nSrcLineBytes = th.m_nWidth * nBytePP;
	for (ASCInt i = 0; i < th.m_nHeight; ++i)
	{
		memcpy(pDestBits, pSrcBits, nSrcLineBytes);
		pSrcBits += nSrcLineBytes;
		pDestBits += pBmp->GetLineBytes();
	}

	free(pTgaBits);
	return true;
}