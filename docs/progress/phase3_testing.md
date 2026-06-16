# Phase 3 - Testing

## Completed Tests

### Gaussian

- Black image invariant test

Description:
A completely black image was processed through the Gaussian blur stage.

Expected Result:
All output pixels remain zero.

Status:
PASS

- Impulse response test

Description:
A single 255-valued pixel was placed at the center of the image while all remaining pixels were zero.

Expected Result:
The output matches the normalized 5x5 Gaussian kernel response.

Status:
PASS

- Uniform image invariant test

Description:
A uniform image with all pixels equal to 100 was processed through the Gaussian blur stage.

Expected Result:
All interior pixels remain equal to 100 after filtering.

Status:
PASS

### Sobel

- Uniform image invariant test

Description:
A uniform image with all pixels equal to 100 was processed through Sobel X and Sobel Y.

Expected Result:
All interior gradient values are zero because no intensity change exists.

Status:
PASS

- Vertical edge response test

Description:
A synthetic vertical edge was created with dark pixels on the left and bright pixels on the right.

Expected Result:
Sobel X produces a positive response of 1020 at interior edge locations.

Status:
PASS

- Horizontal edge response test

Description:
A synthetic horizontal edge was created with dark pixels above and bright pixels below.

Expected Result:
Sobel Y produces a positive response of 1020 at interior edge locations.

Status:
PASS

### Magnitude

- L1 and L2 reference value test

Description:
Known Gx and Gy values were provided and the resulting L1 and L2 magnitudes were compared against manually calculated reference values.

Expected Result:
L1 = {7, 17, 23}
L2 = {5, 13, 17}

Status:
PASS

- Saturation test

Description:
Large gradient values were applied to verify output clamping behavior.

Expected Result:
Both L1 and L2 magnitudes saturate at 255.

Status:
PASS

### Direction

- Canonical angle quantization test

Description:
Representative gradient vectors corresponding to 0°, 45°, 90°, and 135° were provided.

Expected Result:
Directions are quantized to {0, 45, 90, 135} respectively.

Status:
PASS

### Non-Maximum Suppression

- Local maximum preservation test

Description:
A center pixel with magnitude 20 was placed between two smaller neighbors along the comparison direction.

Expected Result:
The center pixel is preserved because it is the local maximum.

Status:
PASS

- Non-maximum suppression test

Description:
A center pixel with magnitude 10 was placed beside a stronger neighbor with magnitude 20 along the comparison direction.

Expected Result:
The center pixel is suppressed and set to zero.

Status:
PASS

### Double Threshold

- Pixel classification test

Description:
Three pixels were selected to represent values below the low threshold, between the thresholds, and above the high threshold.

Expected Result:
Outputs are classified as:
0 (non-edge),
128 (weak edge),
255 (strong edge).

Status:
PASS

### Hysteresis

- Connected weak edge promotion test

Description:
A weak edge pixel was placed adjacent to a strong edge pixel.

Expected Result:
The weak edge is promoted to a strong edge through hysteresis tracking.

Status:
PASS

- Isolated weak edge removal test

Description:
A weak edge pixel was placed without any connected strong edge pixels.

Expected Result:
The weak edge is discarded and the output remains zero.

Status:
PASS
