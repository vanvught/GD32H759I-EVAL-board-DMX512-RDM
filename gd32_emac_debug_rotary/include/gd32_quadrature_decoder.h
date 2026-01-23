/*
 * gd32_quadrature_decoder.h
 */

#ifndef GD32_QUADRATURE_DECODER_H_
#define GD32_QUADRATURE_DECODER_H_

#include <cstdint>

void gd32_quadrature_decoder_begin();

bool gd32_quadrature_decoder_read(uint32_t& nCount, uint32_t& nDirection);

#endif /* GD32_QUADRATURE_DECODER_H_ */
