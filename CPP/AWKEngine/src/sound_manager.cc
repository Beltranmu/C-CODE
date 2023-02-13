
//#include "sound_manager.h"
#include "internals.h"
#include "common.h"
#include <stdio.h>
#include <conio.h>
#include "GLFW/glfw3.h"

AWK::SoundManager::SoundManager() { _data = new Data(); mode = 0; }

AWK::SoundManager::~SoundManager() { delete _data; }

void AWK::SoundManager::init(){

  // Initialize channels to 0
  for (int i = 0; i < kNChannels; ++i){

    _data->_channel[i] = 0;

  }
  _data->id_sound = 0;


  // System creation
  _data->_result = FMOD::System_Create(&_data->_soundSystem);
  _data->_soundSystem->init(kNChannels, FMOD_INIT_NORMAL, NULL);


  _data->_result = _data->_soundSystem->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &_data->dsp);
  ERRCHECK(_data->_result);
  _data->_result = _data->dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, 440.0f); /* Musical note 'A' */
  ERRCHECK(_data->_result);

  _data->pianof[0] =  26162.0f;
    _data->pianof[1] =  27718.0f;
  _data->pianof[2] =  29366.0f;
    _data->pianof[3] =  31112.0f;
  _data->pianof[4] =  32962.0f;

  _data->pianof[5] =  34922.0f;
    _data->pianof[6] =  36999.0f;
  _data->pianof[7] =  39199.0f;
    _data->pianof[8] =  41530.0f;
  _data->pianof[9] =  44000.0f;
    _data->pianof[10] = 44616.0f;
  _data->pianof[11] = 49388.0f;
  

}

void AWK::SoundManager::musicInput(){
  //Change Mode
  if (Common_BtnPress(BTN_ACTION1)) {
    system("cls");
    printf("Mode : Crossfading\n");
    printf("Keys : Q\n");
    printf("Change Mode: 1-2-3-4\n");
    printf("1 -> Crossfading\n");
    printf("2 -> Layering\n");
    printf("3 -> Branching\n");
    printf("4 -> Piano\n");
    ResetMode(mode, 1);
    mode = 1;
  }
  if (Common_BtnPress(BTN_ACTION2)) {
    system("cls");
    printf("Mode : Layering\n");
    printf("Keys : 5-6-7\n");
    printf("Change Mode: 1-2-3-4\n");
    printf("1 -> Crossfading\n");
    printf("2 -> Layering\n");
    printf("3 -> Branching\n");
    printf("4 -> Piano\n");
    ResetMode(mode,2);
    mode = 2;
  }
  if (Common_BtnPress(BTN_ACTION3)) {
    system("cls");
    printf("Mode : Branching\n");
    printf("Keys : Q: b->a\n      W a->b\n");
    printf("Change Mode: 1-2-3-4\n");
    printf("1 -> Crossfading\n");
    printf("2 -> Layering\n");
    printf("3 -> Branching\n");
    printf("4 -> Piano\n");
    ResetMode(mode,3);
    mode = 3;
  }
  if (Common_BtnPress(BTN_ACTION4)) {
    system("cls");
    printf("Mode : Piano\n"); 
    printf("Change Mode: 1-2-3-4\n");
    printf("1 -> Crossfading\n");
    printf("2 -> Layering\n");
    printf("3 -> Branching\n");
    printf("4 -> Piano\n");
    ResetMode(mode,4);
    mode = 4;
  }


  switch (mode)
  {
    //Crossfading
    case 1:
      if (Common_BtnPress(BTN_ACTIONQ)){
        if(!_data->crossfading_mode){
          glfwSetTime(0.0f);
          crossfading(1, 0, 2.0f);
        }
        else{
          glfwSetTime(0.0f);
          crossfading(0, 1, 2.0f);
        }
        _data->crossfading_mode = !_data->crossfading_mode;
      }
      
      break;

    //Layering
    case 2:
      if (Common_BtnPress(BTN_ACTION5)) {
        toggleLayer(layer_1, 0);
      }
      if (Common_BtnPress(BTN_ACTION6)) {
        toggleLayer(layer_1, 1);
      }
      if (Common_BtnPress(BTN_ACTION7)) {
        toggleLayer(layer_1, 2);
      }
      break;
    //Branching
    case 3:
      if (Common_BtnPress(BTN_ACTIONQ)){
        changeBrach(3, 2);
      }
      if (Common_BtnPress(BTN_ACTIONW)){
        changeBrach(1, 4);
      }
      break;


    //Piano
    case 4:
      //S -> DO
      if (Common_BtnPress(BTN_ACTIONS)) {
        printf("DO\n");
        Piano(0);
      }
      //E -> DO S / RE B
      if (Common_BtnPress(BTN_ACTIONE)) {
        printf("DO S / RE B\n");
        Piano(1);
      }
      //D -> RE
      if (Common_BtnPress(BTN_ACTIOND)) {
        printf("RE\n");
        Piano(2);
      }
      //R -> RE S / MI B
      if (Common_BtnPress(BTN_ACTIONR)) {
        printf("RE S / MI B\n");
        Piano(3);
      }
      //F -> MI
      if (Common_BtnPress(BTN_ACTIONF)) {
        printf("MI\n");
        Piano(4);
      }

      //G -> FA
      if (Common_BtnPress(BTN_ACTIONG)) {
        printf("FA\n");
        Piano(5);
      }
      //Y -> FA S / SOL B
      if (Common_BtnPress(BTN_ACTIONY)) {
        printf("FA S / SOL B\n");
        Piano(6);
      }
      //H -> SOL
      if (Common_BtnPress(BTN_ACTIONH)) {
        printf("SOL\n");
        Piano(7);
      }
      //U -> SOL S/ LA B
      if (Common_BtnPress(BTN_ACTIONU)) {
        printf("SOL S/ LA B\n");
        Piano(8);
      }
      //J -> LA
      if (Common_BtnPress(BTN_ACTIONJ)) {
        printf("LA\n");
        Piano(9);
      }
      //I -> LA S/ SI B
      if (Common_BtnPress(BTN_ACTIONI)) {
        printf("LA S/ SI B\n");
        Piano(10);
      }
      //K -> SI
      if (Common_BtnPress(BTN_ACTIONK)) {
        printf("SI\n");
        Piano(11);
      }
      break;
  }
}

void AWK::SoundManager::ResetMode(int previous_mode, int next_mode){
  //Stops
  switch (previous_mode)
  {
    case 1: _data->_channel[0]->stop();
            _data->_channel[1]->stop(); break;
    
    case 2: _data->_channel[2]->stop(); 
            _data->_channel[3]->stop(); 
            _data->_channel[4]->stop();  break;

    case 3: _data->_channel[31]->stop(); break;

    case 4: _data->piano_channel[0]->stop(); break;
  }
  switch (next_mode)
  {
    case 1: _data->_soundSystem->playSound(_data->_sound[0], 0, false, &_data->_channel[0]);; break;
    case 2:  layer_1.init(3, sound_id); playLayer(layer_1); break;
    case 3:  playBrach(0); break;
    case 4: break;
  }
}

void AWK::SoundManager::loadSound(const char* path){
  if(_data->id_sound < kNChannels){
    _data->_result = _data->_soundSystem->createSound(Common_MediaPath(path), FMOD_DEFAULT, 0, &_data->_sound[_data->id_sound]);
    ERRCHECK(_data->_result);
    _data->_result = _data->_sound[_data->id_sound]->setMode(FMOD_LOOP_NORMAL);
    _data->id_sound++;
  }

}

void AWK::SoundManager::play(){

  _data->_result = _data->_soundSystem->playSound(_data->_sound[0], 0, false, &_data->_channel[0]);

}

void AWK::SoundManager::crossfading(int32_t id_fade_in, int32_t id_fade_out, float time){

  _data->crossfading_active = true;
  _data->sound_fade_in_id = id_fade_in;
  _data->sound_fade_out_id = id_fade_out;
  _data->crossfade_time = time;
  _data->_soundSystem->playSound(_data->_sound[id_fade_in], 0, false, &_data->_channel[id_fade_in]);


}

void AWK::SoundManager::update(float time)
{
  float time_ = 0.0f;
  _data->_soundSystem->update();

  ///CROSSFADING
  if (mode == 1) {
    if (_data->crossfading_active == true) {
      time_ = time / _data->crossfade_time;
      if (time_ > 1) {
        _data->_channel[_data->sound_fade_out_id]->stop();
        _data->crossfading_active = false;
      }
      else {
        _data->_channel[_data->sound_fade_in_id]->setVolume(time_);
        _data->_channel[_data->sound_fade_out_id]->setVolume(1 - time_);
      }
    }
  }

  /// BRANCHING
  if (mode == 3) {
    
      _data->_channel[kNChannels - 1]->isPlaying(&_data->end_branch);
      if(!_data->end_branch){
        //Transition finished, go to the next tree
        if(_data->changed &&  (_data->posible_next_stage == _data->transition)){
         playBrach(_data->transition);
          _data->changed = false;
        }else{
          playBrach(_data->default_next_stage);
        }
      }
  }
}


void AWK::SoundManager::freeSM(){
  int i = 0;
   for (i ;  i< _data->id_sound; ++i){
     _data->_result = _data->_sound[i]->release();
   }
  _data->_result = _data->_soundSystem->close();
  _data->_result = _data->_soundSystem->release();

}

void AWK::SoundManager::Layering::init(int32_t nl, int32_t* id_sounds){
  id_chanel = (int32_t*)malloc(sizeof(int32_t) * nl);
  state = (bool*)malloc(sizeof(bool) * nl);
  n_layers = nl;
  for(int i =0 ; i < nl; ++i){
    *(id_chanel+i) = *(id_sounds+i);
    *(state + i) = true;
  } 
}

void AWK::SoundManager::playLayer(Layering &layer){
  
  for (int i = 0; i < layer.n_layers; ++i) {
    _data->_result = _data->_soundSystem->playSound(_data->_sound[layer.id_chanel[i]], 0, false, &_data->_channel[layer.id_chanel[i]]);
    ERRCHECK(_data->_result);
  }
}

void AWK::SoundManager::activateLayer(Layering& layer, int32_t id_chanel) {
  _data->_channel[layer.id_chanel[id_chanel]]->setVolume(1);
  
}

void AWK::SoundManager::desactivateLayer(Layering& layer, int32_t id_chanel) {
  _data->_channel[layer.id_chanel[id_chanel]]->setVolume(0);
  
}

void AWK::SoundManager::toggleLayer(Layering& layer, int32_t id_chanel) {
  layer.state[id_chanel] = !layer.state[id_chanel];
  if(layer.state[id_chanel]){
    _data->_channel[layer.id_chanel[id_chanel]]->setVolume(1);
  }
  else {
    _data->_channel[layer.id_chanel[id_chanel]]->setVolume(0);
  }
  
}

void AWK::SoundManager::initBranching(int32_t nodes){
 
  branching_1.init(nodes);
  _data->changed = false;
  _data->next_stage = 0;

  //Load Songs
  Data::Branching *new_node;
  new_node = (Data::Branching*)malloc(sizeof(Data::Branching)*nodes);

  new_node->type = Data::Branching::SoundType::kSoundType_Combat;
  new_node->id = 0;
  new_node->sound_id = _data->id_sound;
  loadSound("../../data/sounds/Branching_B_0.ogg");
  _data->_sound[_data->id_sound - 1]->setMode(FMOD_LOOP_OFF);
  new_node->nextList = -1;
  new_node->defaultNext = 1;
  branching_1.insertLast((void*)new_node, sizeof(Data::Branching));
  


  //-------------------------------------
  (new_node+1)->type = Data::Branching::SoundType::kSoundType_Combat;
  (new_node + 1)->id = 1;
  (new_node + 1)->sound_id = _data->id_sound;
  loadSound("../../data/sounds/Branching_B_1.ogg");
  _data->_sound[_data->id_sound - 1]->setMode(FMOD_LOOP_OFF);
  (new_node + 1)->nextList = 2;
  (new_node + 1)->defaultNext = 0;
  branching_1.insertLast((void*)(new_node + 1), sizeof(Data::Branching));


  //-------------------------------------
  (new_node + 2)->type = Data::Branching::SoundType::kSoundType_Transition;
  (new_node + 2)->id = 2;
  (new_node + 2)->sound_id = _data->id_sound;
  loadSound("../../data/sounds/Branching_Transition.ogg");
  _data->_sound[_data->id_sound - 1]->setMode(FMOD_LOOP_OFF);
  (new_node + 2)->nextList = -1;
  (new_node + 2)->defaultNext = 3;
  branching_1.insertLast((void*)(new_node + 2), sizeof(Data::Branching));


  //-------------------------------------
  (new_node + 3)->type = Data::Branching::SoundType::kSoundType_Normal;
  (new_node + 3)->id = 3;
  (new_node + 3)->sound_id = _data->id_sound;
  loadSound("../../data/sounds/Branching_A_0.ogg");
  _data->_sound[_data->id_sound - 1]->setMode(FMOD_LOOP_OFF);
  (new_node + 3)->nextList = 4;
  (new_node + 3)->defaultNext = 3;
  branching_1.insertLast((void*)(new_node + 3), sizeof(Data::Branching));

  (new_node + 4)->type = Data::Branching::SoundType::kSoundType_Transition;
  (new_node + 4)->id = 4;
  (new_node + 4)->sound_id = (new_node + 2)->sound_id;
  (new_node + 4)->nextList = -1;
  (new_node + 4)->defaultNext = 1;
  branching_1.insertLast((void*)(new_node + 4), sizeof(Data::Branching));
}

void AWK::SoundManager::playBrach(int32_t starting_id) {
  Data::Branching* new_node = (Data::Branching*)branching_1.at(starting_id);

  _data->_soundSystem->playSound(_data->_sound[new_node->sound_id],
    0, false, &_data->_channel[31]);

  _data->default_next_stage = new_node->defaultNext;
  _data->posible_next_stage = new_node->nextList;
}

void AWK::SoundManager::changeBrach(int32_t new_stage, int32_t transition) {
  _data->changed = true;

  _data->next_stage = new_stage;
  _data->transition = transition;
}

void AWK::SoundManager::Piano(int32_t note){
  if (_data->piano_channel[0])
  {
    _data->_result = _data->piano_channel[0]->stop();
    ERRCHECK(_data->_result);
  }

  _data->_result = _data->_soundSystem->playDSP(_data->dsp, 0, true, &_data->piano_channel[0]);
  ERRCHECK(_data->_result);
  _data->_result = _data->piano_channel[0]->setVolume(0.33f);
  ERRCHECK(_data->_result);
  _data->_result = _data->dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 0);
  ERRCHECK(_data->_result);
  _data->_result = _data->piano_channel[0]->setFrequency(_data->pianof[note]);
  ERRCHECK(_data->_result);
  _data->_result = _data->piano_channel[0]->setPaused(false);
  ERRCHECK(_data->_result);
}

