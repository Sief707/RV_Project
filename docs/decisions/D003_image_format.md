# D003 - Image Framework

## Objective

Provide a lightweight grayscale image container for the Canny edge detection pipeline.

## Design

The project uses an Image class containing:

- width
- height
- pixel buffer

Pixel data is stored as:

std::vector<uint8_t>

Each pixel represents one grayscale intensity value.

## Rationale

The project specification requires raw grayscale images.

Image processing stages (Gaussian, Sobel, Magnitude, Direction) will operate on this common image representation.

Templates will be applied at the convolution layer rather than the image container itself.

## Current Status

PASS

The Image class interface and constructor implementation have been completed and integrated into the host build system.


## Convolution Engine

Status:

PASS

A generic scalar convolution engine has been implemented.

Characteristics:

- Template-based accumulator type
- Row-major kernel storage
- Zero-padding boundary handling
- Verified with deterministic 3x3 convolution test

The convolution engine serves as the common foundation for Gaussian and Sobel filters.


## Convolution Redesign

Status:

PASS

The original convolution engine returned uint8_t image data directly.

This design was replaced with an intermediate ImageInt32 representation to preserve accumulated convolution results.

Benefits:

- Supports Gaussian normalization
- Preserves Sobel negative gradients
- Supports magnitude computation without information loss

Pipeline:

Image
    ->
Convolution
    ->
ImageInt32
    ->
Post Processing
    ->
Image


## Gaussian Verification

Status:

PASS

The Gaussian blur implementation was verified using an impulse-image test.

Input:

- Single 255-valued pixel at image center
- Remaining pixels set to zero

Observed output matched the expected normalized 5x5 Gaussian kernel response.

Verification confirmed:

- Kernel coefficients
- Kernel orientation
- Convolution correctness
- Normalization correctness


## Sobel Verification

Status:

PASS

The Sobel-X operator was verified using a synthetic vertical-edge image.

Observed response:

- Positive gradients at the intensity transition
- Maximum response of 1020
- Correct preservation of negative gradients

Verification confirmed:

- Sobel kernel coefficients
- Gradient sign correctness
- ImageInt32 intermediate representation


## Gradient Magnitude Verification

Status:

PASS

The gradient magnitude stage was verified using known Pythagorean triples.

Test vectors:

- (3,4)  -> 5
- (5,12) -> 13
- (8,15) -> 17

Observed output matched expected Euclidean magnitudes.

Verification confirmed:

- Magnitude computation
- Square-root implementation
- ImageInt32 interoperability


## Direction Quantization Verification

Status:

PASS

The direction quantization stage was verified using known gradient vectors.

Test vectors:

- (10, 0)    ->   0°
- (10,10)    ->  45°
- (0,10)     ->  90°
- (-10,10)   -> 135°

Observed output:

0 45 90 135

Verification confirmed:

- atan2 computation
- Angle normalization to [0,180)
- Quantization into four direction bins


## Magnitude Comparison

Status:

PASS

Both L1 and L2 gradient magnitude formulations were implemented and verified.

Definitions:

L1 = |Gx| + |Gy|

L2 = sqrt(Gx*Gx + Gy*Gy)

Verification vectors:

(3,4)
(5,12)
(8,15)

Observed results:

L1: 7 17 23
L2: 5 13 17

The implementation satisfies the Phase 2 requirement to compare L1 and L2 magnitude formulations.

