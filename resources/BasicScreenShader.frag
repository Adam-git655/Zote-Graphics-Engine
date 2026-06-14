#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float kernelOffset = 1.0 / 300.0;

uniform bool inversion = false;
uniform	bool grayscale = false;
uniform bool sharpen = false;
uniform bool blur = false;
uniform bool edgeDetection = false;

vec3 PostProcessKernel(float kernel[9])
{
    vec2 offsets[9] = vec2[](
        vec2(-kernelOffset,  kernelOffset), // top-left
        vec2( 0.0f,    kernelOffset), // top-center
        vec2( kernelOffset,  kernelOffset), // top-right
        vec2(-kernelOffset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( kernelOffset,  0.0f),   // center-right
        vec2(-kernelOffset, -kernelOffset), // bottom-left
        vec2( 0.0f,   -kernelOffset), // bottom-center
        vec2( kernelOffset, -kernelOffset)  // bottom-right    
    );

    vec3 sampleTex[9];
    for (int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));
    }

    vec3 color = vec3(0.0);
    for (int i = 0; i < 9; i++)
    {
        color += sampleTex[i] * kernel[i];
    }

	return color;
}

void main()
{
    vec3 result = vec3(texture(screenTexture, TexCoords));

    if (inversion)
    {
        result = vec3(1.0 - texture(screenTexture, TexCoords));
    }
    else if (grayscale) 
    {
        FragColor = texture(screenTexture, TexCoords);
        float avg = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
        result = vec3(avg, avg, avg);
    }
    else if (sharpen)
    {
        float kernel[9] = float[](
            -1, -1, -1,
            -1,  9, -1,
            -1, -1, -1
        );
        result = PostProcessKernel(kernel);
    }
    else if (blur)
    {
        float kernel[9] = float[](
            1.0 / 16, 2.0 / 16, 1.0 / 16,
            2.0 / 16, 4.0 / 16, 2.0 / 16,
            1.0 / 16, 2.0 / 16, 1.0 / 16  
        );
        result = PostProcessKernel(kernel);
    }
    else if (edgeDetection)
    {
        float kernel[9] = float[](
            1, 1, 1,
            1, -8, 1,
            1, 1, 1
        );
        result = PostProcessKernel(kernel);
    }

	FragColor = vec4(result, 1.0);
}