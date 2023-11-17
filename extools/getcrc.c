#define CRC_POLY_LOW 33
#define CRC_POLY_HIGH 16

unsigned getcrc(fpacket,flen)
char *fpacket;
int flen;
{
  unsigned int w,ch,j,savec,hicrc,locrc;

  hicrc = locrc = 0;

  for (w = 0 ; w < flen ; w ++) {
    ch = *fpacket++;
    for (j = 1 ; j <= 8 ; j ++) {
      ch *= 2;
      savec = ch;
      ch &= 0x00ff;
      hicrc *= 2;
      locrc *= 2;
      if (savec > 255) locrc |= 1;
      if (locrc > 255) hicrc |= 1;
      if (hicrc > 255) {
	hicrc ^= CRC_POLY_HIGH;
	locrc ^= CRC_POLY_LOW;
      }
      hicrc &= 0x00ff;
      locrc &= 0x00ff;
    }
  }
  return((hicrc << 8) | locrc);
}
