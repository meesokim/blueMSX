// This file is taken from the openMSX project. 
// The file has been modified to be built in the blueMSX environment.

#ifndef __YM2413_HH__
#define __YM2413_HH__

#include <string>

using namespace std;


typedef unsigned long  EmuTime;
typedef unsigned char  _byte;
typedef unsigned short word;

extern "C" {
#include "AudioMixer.h"
}


#ifndef OPENMSX_SOUNDDEVICE
#define OPENMSX_SOUNDDEVICE

class SoundDevice
{
	public:
        SoundDevice() : internalMuted(true) {}
		void setVolume(short newVolume) {
	        setInternalVolume(newVolume);
        }

	protected:
		virtual void setInternalVolume(short newVolume) = 0;
        void setInternalMute(bool muted) { internalMuted = muted; }
        bool isInternalMuted() const { return internalMuted; }
	public:
		virtual void setSampleRate(int newSampleRate, int Oversampling) = 0;
		virtual int* updateBuffer(int length) = 0;

	private:
		bool internalMuted;
};

#endif

#ifndef OPENMSX_YM2413BASE
#define OPENMSX_YM2413BASE

class OpenYM2413Base : public SoundDevice
{
public:
    OpenYM2413Base() {}
    virtual ~OpenYM2413Base() {}
		
	virtual void reset(const EmuTime &time) = 0;
	virtual void writeReg(_byte r, _byte v, const EmuTime &time) = 0;
    virtual _byte peekReg(_byte r) = 0;
	
	virtual int* updateBuffer(int length) = 0;
	virtual void setSampleRate(int sampleRate, int Oversampling) = 0;

    virtual void loadState() = 0;
    virtual void saveState() = 0;
};

#endif

class Slot
{
	public:
		Slot();

		inline int volume_calc(_byte LFO_AM);
		inline void KEY_ON (_byte key_set);
		inline void KEY_OFF(_byte key_clr);

		_byte ar;	// attack rate: AR<<2
		_byte dr;	// decay rate:  DR<<2
		_byte rr;	// release rate:RR<<2
		_byte KSR;	// key scale rate
		_byte ksl;	// keyscale level
		_byte ksr;	// key scale rate: kcode>>KSR
		_byte mul;	// multiple: mul_tab[ML]

		// Phase Generator
		int phase;	// frequency counter
		int freq;	// frequency counter step
		_byte fb_shift;	// feedback shift value
		int op1_out[2];	// slot1 output for feedback

		// Envelope Generator
		_byte eg_type;	// percussive/nonpercussive mode
		_byte state;	// phase type
		int TL;		// total level: TL << 2
		int TLL;	// adjusted now TL
		int volume;	// envelope counter
		int sl;		// sustain level: sl_tab[SL]

		_byte eg_sh_dp;	// (dump state)
		_byte eg_sel_dp;	// (dump state)
		_byte eg_sh_ar;	// (attack state)
		_byte eg_sel_ar;	// (attack state)
		_byte eg_sh_dr;	// (decay state)
		_byte eg_sel_dr;	// (decay state)
		_byte eg_sh_rr;	// (release state for non-perc.)
		_byte eg_sel_rr;	// (release state for non-perc.)
		_byte eg_sh_rs;	// (release state for perc.mode)
		_byte eg_sel_rs;	// (release state for perc.mode)

		_byte key;	// 0 = KEY OFF, >0 = KEY ON

		// LFO
		_byte AMmask;	// LFO Amplitude Modulation enable mask
		_byte vib;	// LFO Phase Modulation enable flag (active high)

		int wavetable;	// waveform select
};

class Channel
{
	public:
		Channel();
		inline int chan_calc(_byte LFO_AM);
		inline void CALC_FCSLOT(Slot *slot);

		Slot slots[2];
		// phase generator state
		int block_fnum;	// block+fnum
		int fc;		// Freq. freqement base
		int ksl_base;	// KeyScaleLevel Base step
		_byte kcode;	// key code (for key scaling)
		_byte sus;	// sus on/off (release speed in percussive mode)
};

class OpenYM2413 : public OpenYM2413Base
{
	public:
		OpenYM2413(const string &name, short volume, const EmuTime &time);
		virtual ~OpenYM2413();
		
		virtual void reset(const EmuTime &time);
		virtual void writeReg(_byte r, _byte v, const EmuTime &time);
        virtual _byte peekReg(_byte r) { return regs[r]; }
		
		virtual void setInternalVolume(short newVolume);
		virtual int* updateBuffer(int length);
		virtual void setSampleRate(int sampleRate, int Oversampling);

        virtual void loadState();
        virtual void saveState();

	private:
        int filter(int input);
		void checkMute();
		bool checkMuteHelper();
		
		void init_tables();
		
		inline void advance_lfo();
		inline void advance();
		inline int rhythm_calc(bool noise);

		inline void set_mul(_byte slot, _byte v);
		inline void set_ksl_tl(_byte chan, _byte v);
		inline void set_ksl_wave_fb(_byte chan, _byte v);
		inline void set_ar_dr(_byte slot, _byte v);
		inline void set_sl_rr(_byte slot, _byte v);
		void load_instrument(_byte chan, _byte slot, _byte* inst);
		void update_instrument_zero(_byte r);
		void setRhythmMode(bool newMode);

        int buffer[AUDIO_MONO_BUFFER_SIZE];
        int oplOversampling;

        int in[5];

        _byte regs[0x40];

		Channel channels[9];	// OPLL chips have 9 channels
		_byte instvol_r[9];		// instrument/volume (or volume/volume in percussive mode)

        short maxVolume;

		unsigned int eg_cnt;		// global envelope generator counter
		unsigned int eg_timer;		// global envelope generator counter works at frequency = chipclock/72
		unsigned int eg_timer_add;	    	// step of eg_timer

		bool rhythm;			// Rhythm mode

		// LFO
		unsigned int lfo_am_cnt;
		unsigned int lfo_am_inc;
		unsigned int lfo_pm_cnt;
		unsigned int lfo_pm_inc;

		int noise_rng;		// 23 bit noise shift register
		int noise_p;		// current noise 'phase'
		int noise_f;		// current noise period

		// instrument settings
		//   0     - user instrument
		//   1-15  - fixed instruments
		//   16    - bass drum settings
		//   17-18 - other percussion instruments
		_byte inst_tab[19][8];

		int fn_tab[1024];		// fnumber->increment counter

		_byte LFO_AM;
		_byte LFO_PM;
};

#endif

