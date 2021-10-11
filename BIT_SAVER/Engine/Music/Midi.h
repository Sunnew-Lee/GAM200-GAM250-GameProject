#pragma once
#include <vector>
#include <map>
#include <iostream>
class MidiEvent {
public:
    MidiEvent() {}
    ~MidiEvent() {}

    std::map<int, std::vector<long double>> MidiSetUp(std::string filename);

private:
    std::vector<MidiEvent> m_events;

    unsigned long       readDtValue(std::istream& inputfile);
    unsigned long       openDtInformation(unsigned char a = 0, unsigned char b = 0, unsigned char c = 0, unsigned char d = 0, unsigned char e = 0);
    unsigned char       readBytes(std::istream& input);
    int                 extractMidi(std::istream& inputfile, std::vector<unsigned char>& array, unsigned char& runningCommand);

    int        tick = 0;     // dt or real MIDI ticks
    int        track = 0;    // original track num of event in MIDI file
    double     seconds = 0;  // calculated time in sec
    int        seq = 0;      // sorting sequence number of the event
};

