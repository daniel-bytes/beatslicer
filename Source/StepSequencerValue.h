#ifndef __STEPSEQUENCERVALUE_H__
#define __STEPSEQUENCERVALUE_H__

#define SEQUENCER_STEP_OFF -1

struct StepSequencerValue
{
public:
	StepSequencerValue()
	{
		value = SEQUENCER_STEP_OFF;
	}

	StepSequencerValue(int value)
	{
		this->value = value;
	}

	bool hasValue(void) const { return value > SEQUENCER_STEP_OFF; }

	int value;
};

struct SequencerStep
{
	SequencerStep(int step, int value)
		: step(step), value(value)
	{
	}

	SequencerStep(int from)
	{
		step = (from >> 16) & 0xFFFF;
		value = from & 0xFFFF;
	}

	operator int()
	{
		return ((step & 0xFFFF) << 16) | (value & 0xFFFF);
	}

	int step;
	int value;
};

#endif //__STEPSEQUENCERVALUE_H__