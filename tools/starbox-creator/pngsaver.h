#ifndef PNGSAVER_H
#define PNGSAVER_H

extern void SaveToPNG(const char * filename, int numChannels, int bitDepth, int width, int height, const unsigned char * data, const char * creationSettingsText);

#endif // PNGSAVER_H
