

#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D Texture;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{           
    vec3 color = texture(Texture, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.06 * color;
    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    float shininess=32.0; //higher that it should be for human skin
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    float specStrength=0.09f; //spread of light
    vec3 lightSpec=vec3(1.0f);// assuming bright white light color vec3(1.0)
    vec3 specular =lightSpec* specStrength * spec; 
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

