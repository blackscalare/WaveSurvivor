#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

void main()
{
    // Set the scale of the glow
    float glowScale = 0.1; // Adjust this value as needed

    // Expand the texture coordinate range to create a border around the texture
    vec2 expandedTexCoord = fragTexCoord * 1.1 - 0.05;

    // Sample the original texture
    vec4 texelColor = texture(texture0, fragTexCoord);

    // Sample the texture from the expanded region to create a glow
    vec4 glowColor = texture(texture0, expandedTexCoord);

    // Calculate the glow strength using a smoothstep function
    float glowStrength = smoothstep(0.5 - glowScale, 0.5 + glowScale, length(expandedTexCoord - vec2(0.5, 0.5)));

    // Mix the glow color with the original texture
    finalColor = mix(texelColor, texelColor + glowColor, glowStrength);

    // Apply diffuse color if needed
    finalColor *= colDiffuse;

    // Set the alpha to the original texture alpha
    finalColor.a = texelColor.a;
}