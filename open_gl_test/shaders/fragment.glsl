#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D ourTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform bool isPlanet;


void main()
{
    // Ambient
    // constant base light nearly dark
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * vec3(1.0);

    // Diffuse
    // the surface reacts to light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    // Specular
    // shiny spot
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    //dot(viewDir, reflectDir) = check if the camera sees the reflection
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0);

    vec3 lighting = ambient + diffuse + specular;
    vec3 texColor = texture(ourTexture, TexCoord).rgb;

    if (isPlanet)
    {
        FragColor = vec4(texColor * 1.3 , 1.0); // make it a bit brighter
    }
    else
    {
        FragColor = vec4(lighting * texColor, 1.0);
    }
}
