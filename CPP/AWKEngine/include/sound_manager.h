/**
 * @brief Sound Manager Class, Graphic Engine Development , ESAT - 2022
 * @file soundmanager.h 
 * @author Ricardo Beltrán Muriel <beltranmu@esat-alumni.com> and Javier Benito Abolafio <benitoab@esat-alumni.com>
 * demo video https://youtu.be/5gG9HpHjuXM 
 */




#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__ 1

#include <stdint.h>
#include <stdlib.h>
#include "adt_vector.h"
namespace AWK{
  /**
    *@brief SoundManager class.
  */
  class SoundManager{
  
    public:
      SoundManager();
      ~SoundManager();
      int32_t mode;
      /**
       *@brief init the soundmanager
      */
      void init();

      /**
       *@brief load a new sound
       *@param const char* path, the path of the sound
      */
      void loadSound(const char* path);

      /**
       *@brief play the sound 0 in the channel 0
      */
      void play();

      /**
       *@brief function update to have a control of the sound effects
       *@param float time, delta time of each loop
      */
      void update(float time);

      /**
       *@brief function where the input for the sound effect are caught
      */
      void musicInput();

      /**
       *@brief Make the necessary changes to change between the different modes of the demo
       *@param int previous_mode, the id of the current mode
       *@param int next_mode, the id of the mode your are going to change into
      */
      void ResetMode(int previous_mode, int next_mode);

      /**
       *@brief Activate the crossfading between two sounds.
       *@param int32_t id_fade_in, the id of the sound that is going to start playing and increase its volume
       *@param int32_t id_fade_out, the id of the sound that is going decrease its volume and stop playing
       *@param float time = 1.0f, the transition time, default 1 sec
      */
      void crossfading(int32_t id_fade_in, int32_t id_fade_out, float time = 1.0f);

      /**
       *@brief free all the resources
      */
      void freeSM();

      struct Data; 

      /**
       *@brief Layering class. To do the layering effect
      */
      class Layering {
      public:
        Layering() {
          id_chanel = NULL;
          state = NULL;
          n_layers = 0;
        };
        ~Layering() {
          if (NULL != id_chanel) {
            free(id_chanel);
          }

          if (NULL != state) {
            free(state);
          }
        }


        int32_t n_layers;   ///< number of layer for the effect

        int32_t* id_chanel; ///< id for the channels
        bool* state;        ///< state for each channel
        /**
         *@brief Init the Layer.
         *@param int32_t n_layers, number of layers that the effect is going to have 
         *@param int32_t* id_sounds, array for the id of the sound that each layer of the layering will have.
        */
        void init(int32_t n_layers, int32_t* id_sounds);

      };

      /**
       *@brief Play the layer.
       *@param Layering &L, the layer to play 
      */
      void playLayer(Layering &L);

      /**
       *@brief Activate a channel of the layer
       *@param Layering &L, the layer you want to activate a channel
       *@param int32_t id_chanel, the id of the channel
      */
      void activateLayer(Layering &L, int32_t id_chanel);

      /**
       *@brief Deactivate a channel of the layer
       *@param Layering &L, the layer you want to deactivate a channel
       *@param int32_t id_chanel, the id of the channel
      */
      void desactivateLayer(Layering &L, int32_t id_chanel);

      /**
      *@brief Toggle the state of a channel of the layer. If it is activate -> deactivate , deactivate -> activate
      *@param Layering &L, the layer you want to toggle a channel
      *@param int32_t id_chanel, the id of the channel
     */
      void toggleLayer(Layering &L, int32_t id_chanel);
      int sound_id[3] = { 2,3,4 };///< the id of the sounds for the layering
      Layering layer_1;           ///< the layering effect

      //Branching
      Vector branching_1;                 ///< vector with all the nodes of the branching

      /**
      *@brief initiate the branching of a size
      *@param int32_t n_nodes, the number of nodes that the branching will have
      */
      void initBranching(int32_t n_nodes);

      /**
      *@brief play the branching
      *@param int32_t starting_id, the starting node
      */
      void playBrach(int32_t starting_id);

     
      //void playTransition(int32_t starting_id);
      /**
      *@brief change the values for the next branch
      *@param int32_t new_stage, the next node 
      *@param int32_t transition, if the next node has transition 
      */
      void changeBrach(int32_t new_stage, int32_t transition);

      //Piano
      /**
      *@brief Play a sinusoidal sound
      *@param int32_t note, the note to play (DO, RE etc)
      *@param int32_t transition, if the next node has transition
      */
      void Piano(int32_t note);
      

    protected:

    private:
      Data* _data = nullptr;  ///< variable where all data needed for the demo is stored (audio manager, sound sources, etc)

  };

}

#endif  // !__SOUND_MANAGER_H__