#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
// uniform vec2 tilePosition;
// uniform vec4 newColor;


// const float tilesWidth  = 3.0;
// const float tilesHeight = 4.0;

// const float scaleX = 1.0 / tilesWidth;
// const float scaleY = 1.0 / tilesHeight;


void main()
{
    FragColor = texture2D(ourTexture, TexCoord) * vec4(ourColor.xyz, 1.0);

    
    // FragColor = texture2D(ourTexture, TexCoord) * vec4(ourColor.x + newColor.x, ourColor.y + newColor.y, ourColor.z + newColor.z, 1.0);

    

    // FragColor = texture(ourTexture, vec2( (TexCoord.x + tilePosition.x) * scaleX, (TexCoord.y * scaleY) + tilePosition.y * scaleY));
}
