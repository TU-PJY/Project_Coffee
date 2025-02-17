#version 330 core

in vec3           FragPosition;
in vec2           TextureCoord;

out vec4          FragColor;
uniform vec3      ViewPosition;

uniform sampler2D OutTexture;
uniform float     Opacity;
uniform vec3      Color;

uniform bool      BlurState;
uniform float     BlurStrength; 
uniform vec2      TextureSize;

vec4 ComputeBlur() {
    vec4 TotalColor = vec4(0.0);
    float TotalAlpha = 0.0;
    float TotalWeight = 0.0;
    int BlurExecution = 4;
    int KernelSize = (BlurExecution * 2 + 1) * (BlurExecution * 2 + 1);

    for (int Y = -BlurExecution; Y <= BlurExecution; ++Y) {
        for (int X = -BlurExecution; X <= BlurExecution; ++X) {
            vec2 Offset = vec2(float(X), float(Y)) * TextureSize * BlurStrength;
            vec4 sample = texture(OutTexture, TextureCoord + Offset);
            
            float Weight = sample.a;
            TotalColor += sample * Weight;
            TotalAlpha += sample.a;
            TotalWeight += Weight;
        }
    }

    return mix(vec4(0.0), vec4(TotalColor.rgb / TotalWeight, TotalAlpha / float(KernelSize)), step(0.0, TotalWeight));
}

vec4 ComputePixel() {
    vec4 BlurResult = mix(vec4(0.0), ComputeBlur(), float(BlurState));
    vec4 DefaultResult = mix(texture(OutTexture, TextureCoord), vec4(0.0), float(BlurState));
    return mix(DefaultResult, BlurResult, float(BlurState));
}

void main() {
    vec4 FinalResult = ComputePixel();
    FragColor = vec4(FinalResult.rgb + Color.rgb, FinalResult.a * Opacity);
}