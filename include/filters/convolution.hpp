#ifndef CONVOLUTION_HPP
#define CONVOLUTION_HPP

#include <cstdint>
#include <cstring>
#include <vector>
#include "image/image.hpp"

// ─────────────────────────────────────────────────────────────────────────────
//  convolve() — original boundary-check version (Phase 4 baseline)
//
//  The if-continue inside the innermost kx loop introduces control flow.
//  At -O3 the compiler reports: "not vectorized: control flow in loop".
//  This version is the baseline used in the vectorization analysis.
//  It is NOT compiled when -DUSE_PADDED_CONV is defined.
// ─────────────────────────────────────────────────────────────────────────────
#ifndef USE_PADDED_CONV
template<typename AccumulatorType>
ImageInt32 convolve(
    const Image& input,
    const std::vector<AccumulatorType>& kernel,
    uint32_t kernel_width,
    uint32_t kernel_height)
{
    ImageInt32 output(input.width, input.height);

    const int32_t kernel_center_x = kernel_width  / 2;
    const int32_t kernel_center_y = kernel_height / 2;

    for (uint32_t y = 0; y < input.height; ++y)
    {
        for (uint32_t x = 0; x < input.width; ++x)
        {
            AccumulatorType sum = 0;

            for (uint32_t ky = 0; ky < kernel_height; ++ky)
            {
                for (uint32_t kx = 0; kx < kernel_width; ++kx)
                {
                    const int32_t image_x =
                        static_cast<int32_t>(x) +
                        static_cast<int32_t>(kx) - kernel_center_x;
                    const int32_t image_y =
                        static_cast<int32_t>(y) +
                        static_cast<int32_t>(ky) - kernel_center_y;

                    // Control flow — prevents auto-vectorization of kx loop
                    if (image_x < 0 || image_y < 0 ||
                        image_x >= static_cast<int32_t>(input.width) ||
                        image_y >= static_cast<int32_t>(input.height))
                        continue;

                    const uint8_t pixel =
                        input.pixels[image_y * input.width + image_x];
                    sum += pixel * kernel[ky * kernel_width + kx];
                }
            }
            output.pixels[y * input.width + x] = sum;
        }
    }
    return output;
}

#else  // USE_PADDED_CONV defined — use loop-interchanged padded version

// ─────────────────────────────────────────────────────────────────────────────
//  convolve() — padded + loop-interchanged version
//
//  Two improvements over the naive padded version:
//
//  1. PADDING: pre-pads with zeros so boundary check is never needed.
//     Uses memcpy for the row copy — GCC always vectorizes memcpy.
//
//  2. LOOP INTERCHANGE: swaps loop order from (y,x,ky,kx) to (ky,kx,y,x).
//     The innermost loop now iterates over ALL x pixels (512 iterations)
//     instead of kernel columns (5 iterations).
//     A loop of 512 stride-1 accumulations is exactly what the compiler
//     needs to emit SIMD instructions — too short a loop (kx=5) and the
//     compiler decides the vector setup overhead is not worth it.
//
//  Why loop interchange works:
//    Original: for each output pixel, loop over all kernel taps.
//    Interchanged: for each kernel tap, loop over all output pixels.
//    Both produce identical results (accumulation is commutative).
//    But the interchanged form gives the compiler a 512-element inner
//    loop over contiguous memory — a perfect vectorization target.
//
//  Output is mathematically identical to the baseline convolve().
//  The zero-padding matches the boundary condition (out-of-bounds = 0).
// ─────────────────────────────────────────────────────────────────────────────
template<typename AccumulatorType>
ImageInt32 convolve(
    const Image& input,
    const std::vector<AccumulatorType>& kernel,
    uint32_t kernel_width,
    uint32_t kernel_height)
{
    const uint32_t W  = input.width;
    const uint32_t H  = input.height;
    const uint32_t hkw = kernel_width  / 2;
    const uint32_t hkh = kernel_height / 2;

    // ── Padded buffer ────────────────────────────────────────────────────────
    // Dimensions: (W + 2*hkw) × (H + 2*hkh), zero-initialised.
    // Original pixels sit in the centre at offset (hkw, hkh).
    const uint32_t PW = W + 2 * hkw;
    const uint32_t PH = H + 2 * hkh;

    std::vector<uint8_t> padded(PW * PH, 0u);

    // memcpy each row — GCC vectorizes memcpy automatically.
    for (uint32_t row = 0; row < H; ++row)
    {
        std::memcpy(
            padded.data() + (row + hkh) * PW + hkw,
            input.pixels.data() + row * W,
            W);
    }

    // ── Loop-interchanged convolution ────────────────────────────────────────
    // Accumulator image, zero-initialised.
    std::vector<AccumulatorType> acc(W * H, AccumulatorType(0));

    // Outer loops: kernel taps (small, compile-time-like constants for 5×5)
    for (uint32_t ky = 0; ky < kernel_height; ++ky)
    {
        for (uint32_t kx = 0; kx < kernel_width; ++kx)
        {
            const AccumulatorType coeff = kernel[ky * kernel_width + kx];
            if (coeff == 0) continue;  // skip zero-coefficient kernel taps

            // Inner loops: all output pixels — 512×512 = 262144 iterations.
            // The x loop is innermost: contiguous memory, no branches,
            // stride-1 reads from padded[] and writes to acc[].
            // This is the loop the compiler vectorizes.
            for (uint32_t y = 0; y < H; ++y)
            {
                const uint8_t*       src_row = padded.data() + (y + ky) * PW + kx;
                AccumulatorType*     acc_row = acc.data()    +  y        * W;

                // 512-iteration stride-1 loop — no branches — vectorizable
                for (uint32_t x = 0; x < W; ++x)
                    acc_row[x] += static_cast<AccumulatorType>(src_row[x]) * coeff;
            }
        }
    }

    // ── Pack accumulator into output ─────────────────────────────────────────
    ImageInt32 output(W, H);
    for (uint32_t i = 0; i < W * H; ++i)
        output.pixels[i] = acc[i];

    return output;
}

#endif  // USE_PADDED_CONV

#endif  // CONVOLUTION_HPP