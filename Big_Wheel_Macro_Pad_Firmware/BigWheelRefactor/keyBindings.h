#define frameForward KEY_RIGHT
#define frameBack KEY_LEFT
#define increasePlaySpeed KEY_L
#define increaseReverseSpeed KEY_J
#define stopPlayback KEY_K
const int keyAssignments[4]= {KEY_K,KEY_2,KEY_3,KEY_K};
const int knobAssignmets[3][2]={
                          {KEY_J, KEY_L},//Top
                          {KEY_RIGHT_BRACE, KEY_LEFT_BRACE},//midle
                          {KEY_DOWN, KEY_UP}//bottom
                          };


 const int switchAssignments[] = {
  KEY_0, 
  KEY_1, 
  KEY_2, 
  KEY_3, // 3 key on my setup
  KEY_4, //1 key on my setup
  KEY_5, //2 key on my setup
  KEY_6, //down key on my setup
  KEY_7, 
  KEY_8, 
  KEY_9,
  KEY_A, //left key on my setup
  KEY_B, 
  KEY_C, 
  KEY_D, //up arrow on my setup
  KEY_E, // right arrow on my setup
  KEY_F, 
  KEY_G, //escape key on my setup
  KEY_H  // I wired the big knob click into the circuit, even though pressing it is physically impossible
  };
