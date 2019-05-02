#ifndef STREAM_DEFS_H
#define STREAM_DEFS_H

// Widevine H264 clear content
#define WIDEVINE_DASH_H264_CLEAR_SD_URL "https://storage.googleapis.com/wvmedia/clear/h264/tears/tears_sd.mpd"
#define WIDEVINE_DASH_H264_CLEAR_HD_URL "https://storage.googleapis.com/wvmedia/clear/h264/tears/tears_hd.mpd"
#define WIDEVINE_DASH_H264_CLEAR_UHD_URL "https://storage.googleapis.com/wvmedia/clear/h264/tears/tears_uhd.mpd"
#define WIDEVINE_DASH_H264_CLEAR_SD_HD_URL "https://storage.googleapis.com/wvmedia/clear/h264/tears/tears.mpd"
// Widevine H264 secure CENC content
#define WIDEVINE_DASH_H264_SECURE_CENC_SD_URL "https://storage.googleapis.com/wvmedia/cenc/h264/tears/tears_sd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CENC_HD_URL "https://storage.googleapis.com/wvmedia/cenc/h264/tears/tears_hd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CENC_UHD_URL "https://storage.googleapis.com/wvmedia/cenc/h264/tears/tears_uhd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CENC_SD_HD_URL "https://storage.googleapis.com/wvmedia/cenc/h264/tears/tears.mpd"
// Widevine H264 secure CBC1 content
#define WIDEVINE_DASH_H264_SECURE_CBC1_SD_URL "https://storage.googleapis.com/wvmedia/cbc1/h264/tears/tears_aes_cbc1_sd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CBC1_HD_URL "https://storage.googleapis.com/wvmedia/cbc1/h264/tears/tears_aes_cbc1_hd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CBC1_UHD_URL "https://storage.googleapis.com/wvmedia/cbc1/h264/tears/tears_aes_cbc1_uhd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD_URL "https://storage.googleapis.com/wvmedia/cbc1/h264/tears/tears_aes_cbc1.mpd"
// Widevine H264 secure CBCS content
#define WIDEVINE_DASH_H264_SECURE_CBCS_SD_URL "https://storage.googleapis.com/wvmedia/cbcs/h264/tears/tears_aes_cbcs_sd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CBCS_HD_URL "https://storage.googleapis.com/wvmedia/cbcs/h264/tears/tears_aes_cbcs_hd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CBCS_UHD_URL "https://storage.googleapis.com/wvmedia/cbcs/h264/tears/tears_aes_cbcs_uhd.mpd"
#define WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD_URL "https://storage.googleapis.com/wvmedia/cbcs/h264/tears/tears_aes_cbcs.mpd"
// Widevine H265 clear content
#define WIDEVINE_DASH_H265_CLEAR_SD_URL "https://storage.googleapis.com/wvmedia/clear/hevc/tears/tears_sd.mpd"
#define WIDEVINE_DASH_H265_CLEAR_HD_URL "https://storage.googleapis.com/wvmedia/clear/hevc/tears/tears_hd.mpd"
#define WIDEVINE_DASH_H265_CLEAR_UHD_URL "https://storage.googleapis.com/wvmedia/clear/hevc/tears/tears_uhd.mpd"
#define WIDEVINE_DASH_H265_CLEAR_SD_HD_URL "https://storage.googleapis.com/wvmedia/clear/hevc/tears/tears.mpd"
// Widevine H265 secure CENC content
#define WIDEVINE_DASH_H265_SECURE_CENC_SD_URL "https://storage.googleapis.com/wvmedia/cenc/hevc/tears/tears_sd.mpd"
#define WIDEVINE_DASH_H265_SECURE_CENC_HD_URL "https://storage.googleapis.com/wvmedia/cenc/hevc/tears/tears_hd.mpd"
#define WIDEVINE_DASH_H265_SECURE_CENC_UHD_URL "https://storage.googleapis.com/wvmedia/cenc/hevc/tears/tears_uhd.mpd"
#define WIDEVINE_DASH_H265_SECURE_CENC_SD_HD_URL "https://storage.googleapis.com/wvmedia/cenc/hevc/tears/tears.mpd"

#define CLEAR_STREAM_NUMBER 6
#define SECURE_STREAM_NUMBER 12
#define UHD_STREAM_NUMBER 6
#define STREAM_NUMBER 24

typedef enum {
	WIDEVINE_DASH_H264_CLEAR_SD,
	WIDEVINE_DASH_H264_CLEAR_HD,
	WIDEVINE_DASH_H264_CLEAR_SD_HD,
	WIDEVINE_DASH_H265_CLEAR_SD,
	WIDEVINE_DASH_H265_CLEAR_HD,
	WIDEVINE_DASH_H265_CLEAR_SD_HD,

	WIDEVINE_DASH_H264_SECURE_CENC_SD,
	WIDEVINE_DASH_H264_SECURE_CENC_HD,
	WIDEVINE_DASH_H264_SECURE_CENC_SD_HD,
	WIDEVINE_DASH_H264_SECURE_CBC1_SD,
	WIDEVINE_DASH_H264_SECURE_CBC1_HD,
	WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD,
	WIDEVINE_DASH_H264_SECURE_CBCS_SD,
	WIDEVINE_DASH_H264_SECURE_CBCS_HD,
	WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD,
	WIDEVINE_DASH_H265_SECURE_CENC_SD,
	WIDEVINE_DASH_H265_SECURE_CENC_HD,
	WIDEVINE_DASH_H265_SECURE_CENC_SD_HD,
	
	WIDEVINE_DASH_H264_CLEAR_UHD,
	WIDEVINE_DASH_H265_CLEAR_UHD,
	
	WIDEVINE_DASH_H264_SECURE_CENC_UHD,
	WIDEVINE_DASH_H264_SECURE_CBC1_UHD,
	WIDEVINE_DASH_H264_SECURE_CBCS_UHD,
	WIDEVINE_DASH_H265_SECURE_CENC_UHD,
	
	last
} stream;

/*
typedef enum {
	WIDEVINE_DASH_H264_CLEAR_SD,
	WIDEVINE_DASH_H264_CLEAR_HD,
	WIDEVINE_DASH_H264_CLEAR_SD_HD,
	WIDEVINE_DASH_H265_CLEAR_SD,
	WIDEVINE_DASH_H265_CLEAR_HD,
	WIDEVINE_DASH_H265_CLEAR_SD_HD,
	last
} stream_clear;

typedef enum {
	WIDEVINE_DASH_H264_SECURE_CENC_SD,
	WIDEVINE_DASH_H264_SECURE_CENC_HD,
	WIDEVINE_DASH_H264_SECURE_CENC_SD_HD,
	WIDEVINE_DASH_H264_SECURE_CBC1_SD,
	WIDEVINE_DASH_H264_SECURE_CBC1_HD,
	WIDEVINE_DASH_H264_SECURE_CBC1_SD_HD,
	WIDEVINE_DASH_H264_SECURE_CBCS_SD,
	WIDEVINE_DASH_H264_SECURE_CBCS_HD,
	WIDEVINE_DASH_H264_SECURE_CBCS_SD_HD,
	WIDEVINE_DASH_H265_SECURE_CENC_SD,
	WIDEVINE_DASH_H265_SECURE_CENC_HD,
	WIDEVINE_DASH_H265_SECURE_CENC_SD_HD,
	last
} stream_secure;

typedef enum {
	WIDEVINE_DASH_H264_CLEAR_UHD,
	WIDEVINE_DASH_H265_CLEAR_UHD,
	last
} stream_clear_uhd;

typedef enum {
	WIDEVINE_DASH_H264_SECURE_CENC_UHD,
	WIDEVINE_DASH_H264_SECURE_CBC1_UHD,
	WIDEVINE_DASH_H264_SECURE_CBCS_UHD,
	WIDEVINE_DASH_H265_SECURE_CENC_UHD,
	last
} stream_secure_uhd;
*/
#endif