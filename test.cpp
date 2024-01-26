#include "ClearSense.h"
#include "WAV_AIO.h"

int main(int argc, const char* argv[]) {

  int sr = 48000;
  constexpr int n_channels = 2;
  constexpr int n_hop = 128;
  int n_samples = n_hop*3;

  WAV *in = nullptr;
  WAV *out = nullptr;

  if (argc == 1) {
  //  sr = 16000;
   // n_samples = n_hop;
    in = new WAV();
    in->OpenFile("in.wav");
    in->Print();
    sr = in->GetSampleRate();
    if (sr == 0)
      sr = 48000;
    out = new WAV(1, sr);
    out->NewFile("out.wav");
  }
  else if(argc == 3) {
    in = new WAV();
    out = new WAV(1, sr);
    in->OpenFile(argv[1]);
    out->NewFile(argv[2]);
    printf("run %s %s\n",argv[1],argv[2]);
  }

 ClearSense* CSObj = CS_construct(n_channels, n_hop, sr,"mpANC_512.onnx");
 // ClearSense* CSObj = CS_construct(n_channels, n_hop, sr,"mpSEv2_v7.onnx");

  short * buf_in = new short[n_samples * n_channels];
  short * buf_out = new short[n_samples];

  //int inter_update[5][2] = { {74728,143347}, { 233155 , 302203 },  { 390091 , 456830 },  { 572917 , 671615 },  { 765143 , 820602 } };
  int inter_update[6][2] = { { 121487  , 189475 },  { 290900  , 335482 },  { 438579 , 490963 },   { 594059 , 660376 },   { 776847 , 844835 },  { 947931 , 997529 }};

  int idx = 0;
  while(!in->IsEOF()){

    // 20240102_HINT_speech_front_15sec.wav
    /*
    CS_toggle_mpB_update(CSObj, true);
    for (int i = 0; i < 5; i++) {
     if (idx > inter_update[i][0] && idx < inter_update[i][1])
       CS_toggle_mpB_update(CSObj, false);
    }
    */

    // 20240102_HINT_speech_front_noise_right_seg.wav
    CS_toggle_mpB_update(CSObj, true);
    for (int i = 0; i < 6; i++) {
      if (idx > inter_update[i][0] && idx < inter_update[i][1])
        // CS_toggle_mpB_update(CSObj, false);
        ;
    }



    in->ReadUnit(buf_in, n_samples * n_channels);
    CS_process(CSObj, buf_in, buf_out);
    out->Append(buf_out, n_samples);

    idx += n_samples;
  }

  in->Finish();
  out->Finish();

  delete in;
  delete out;
  CS_release(CSObj);


  return 0;
}