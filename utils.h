/** @weakgroup utils
 *  @{
 */
/**
 * @file utils.h
 * @brief some utility functions
 *
 * Copyright 2020 Stanislav V. Vlasov <stanislav.v.v@gmail.com>
 *
 */

/**
 * @brief reverse bits order in number
 * @param num - number for reverse
 * @param bits - size of num in bits (usually 8 or 16)
 * @return reversed number
 */
static inline uint16_t reverse_bits(uint16_t num, uint8_t bits)
{
    uint16_t reverse_num = 0, i;

    for (i = 0; i < bits; i++)
    {
        uint16_t temp = (uint16_t)(num & (1 << i));
        if (temp)
        {
            reverse_num = (uint16_t)(reverse_num | (1 << ((bits - 1) - i)));
        }
    }

    return reverse_num;
}
