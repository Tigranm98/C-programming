/* Copyright Instigate Robotics CJSC 2019 */

#ifndef ASSERT_H
#define ASSERT_H

#ifdef  USE_FULL_ASSERT
void assert_failed(const uint8_t* file, uint32_t line);
#endif 

#endif // ASSERT_H
