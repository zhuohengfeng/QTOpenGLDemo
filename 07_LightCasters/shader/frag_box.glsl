#version 330

out vec4 FragColor;

in vec2 outUV;
in vec3 outFragPos;
in vec3 outNormal;

uniform vec3 view_pos;

struct Material {
    sampler2D m_diffuse; // same as ambient
    sampler2D m_specular; // same as ambient
    int m_shiness;
};
uniform Material myMaterial;

struct Light {
    vec3 m_pos;
    vec3 m_ambient;
    vec3 m_diffuse;
    vec3 m_specular;
};
uniform Light myLight;

void main()
{
    // 环境光
    vec3 _ambient = myLight.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));

    // 漫反射
    vec3 _normal = normalize(outNormal);
    vec3 _lightDir = normalize(myLight.m_pos - outFragPos); // 光入射方向
    float _diff = max(dot(_normal , _lightDir) , 0.0f); // 入射光线和法线的夹角大小
    vec3 _diffuse = myLight.m_diffuse * _diff * vec3(texture(myMaterial.m_diffuse , outUV));

    // 镜面反射
    vec3 _viewDir = normalize(view_pos - outFragPos); // 片元和眼镜的方向
    vec3 _reflectDir = reflect(-_lightDir , _normal); // 反射方向
    float _spec = pow(max(dot(_viewDir , _reflectDir) , 0.0f) , myMaterial.m_shiness); // 计算反射强度
    vec3 _sepcular = myLight.m_specular * _spec * vec3(texture(myMaterial.m_specular , outUV));

    // 总的光照
    FragColor = vec4(_ambient  + _diffuse + _sepcular, 1.0);

}