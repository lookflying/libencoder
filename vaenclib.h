#include <va/va_enc_mpeg2.h>
#include "buffdef.h"

#define MAX_SLICES 128

typedef enum{
	Ivb_VAType_NONE  = 0,
	Ivb_VAType_MPEG2 = 1,
	Ivb_VAType_H264  = 2,
}Ivb_VAType;

struct mpeg2enc_context {
        /* args */
        int rate_control_mode;
        int fps;
        int mode; /* 0:I, 1:I/P, 2:I/P/B */
        VAProfile profile;
        int level;
        int width;
        int height;
        int frame_size;
        int num_pictures;
        int qp;
        FILE *ifp;
        FILE *ofp;
        unsigned char *frame_data_buffer;
        int intra_period;
        int ip_period;
        int bit_rate; /* in kbps */
        VAEncPictureType next_type;
        int next_display_order;
        int next_bframes;
        int new_sequence;
        int new_gop_header;
        int gop_header_in_display_order;

        /* VA resource */
        VADisplay va_dpy;
        VAEncSequenceParameterBufferMPEG2 seq_param;
        VAEncPictureParameterBufferMPEG2 pic_param;
        VAEncSliceParameterBufferMPEG2 slice_param[MAX_SLICES];
        VAContextID context_id;
        VAConfigID config_id;
        VABufferID seq_param_buf_id;                /* Sequence level parameter */
        VABufferID pic_param_buf_id;                /* Picture level parameter */
        VABufferID slice_param_buf_id[MAX_SLICES];  /* Slice level parameter, multil slices */
        VABufferID codedbuf_buf_id;                 /* Output buffer, compressed data */
        VABufferID packed_seq_header_param_buf_id;
        VABufferID packed_seq_buf_id;
        VABufferID packed_pic_header_param_buf_id;
        VABufferID packed_pic_buf_id;
        int num_slice_groups;
        int codedbuf_i_size;
        int codedbuf_pb_size;

        /* thread */
        pthread_t upload_thread_id;
        int upload_thread_value;
        int current_input_surface;
        int current_upload_surface;
};

/*for h.264*/
int init_encoder(int argc, char *argv[]);
int encode_frame(unsigned char *inputdata,struct coded_buff *avc_p);
int close_encoder();
#if 0
int build_packed_pic_buffer(unsigned char **header_buffer);
int build_packed_seq_buffer(unsigned char **header_buffer);
#endif
/*added for mpeg2*/
int init_mpeg2enc(int argc, char* argv[]);
int mpeg2_encode_frame(unsigned char *yuvdata, struct coded_buff *avc_p);
int close_mpeg2_encoder();
int build_packed_pic_buffer_mpeg2(const VAEncSequenceParameterBufferMPEG2 *seq_param,const VAEncPictureParameterBufferMPEG2 *pic_param,unsigned char **header_buffer);
int build_packed_seq_buffer_mpeg2(struct mpeg2enc_context *ctx,const VAEncSequenceParameterBufferMPEG2 *seq_param,unsigned char **header_buffer);
