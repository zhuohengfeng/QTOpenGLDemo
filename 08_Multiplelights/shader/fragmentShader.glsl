#version 330 core

in vec2 outUV;
in vec3 outFragPos;
in vec3 outNormal;

uniform vec3 view_pos;

out vec4 FragColor;

// 定义物体材质，漫反射和镜面反射都是图片纹理
// 其中diffuse和ambient颜色一样
struct Material
{
    sampler2D m_diffuse;
    sampler2D m_specular;

    float m_shiness;
};
uniform Material myMaterial;

// 定向光属性，没有衰减系数
struct DirLight
{
    vec3 m_direction; // 光方向

    vec3 m_ambient; // 环境光
    vec3 m_diffuse; // 漫反射
    vec3 m_specular; // 镜面反射
};

// 点光源
struct PointLight
{
    vec3 m_pos; // 点光源位置

    vec3 m_ambient; // 环境光
    vec3 m_diffuse; // 漫反射
    vec3 m_specular; // 镜面反射

    float m_c;
    float m_l;
    float m_q;
};

// 聚光灯
struct SpotLight
{
    vec3 m_pos; // 光源位置
    vec3 m_direction; // 光源方向
    float m_cutOff; // 内光圈
    float m_outCutOff; // 外光圈

    vec3 m_ambient; // 环境光
    vec3 m_diffuse; // 漫反射
    vec3 m_specular; // 镜面反射

    float m_c;
    float m_l;
    float m_q;
};


#define MAX_POINT_NUMBER 4
uniform DirLight _dirLight;
uniform PointLight _pointLight[MAX_POINT_NUMBER];
uniform SpotLight _spotLight;

// 计算定向光下的环境光，漫反射，镜面反射
vec3 calculateDirLight(DirLight _light, vec3 _normal, vec3 _viewDir)
{
    vec3 _lightDir = normalize(_light.m_direction);
    // 环境光
    vec3 _ambientColor = _light.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));

    // 漫反射
    float _diffuse = max(dot(_normal, -_lightDir), 0.0f);
    vec3 _diffuseColor = _light.m_diffuse * _diffuse * vec3(texture(myMaterial.m_diffuse, outUV));

    // 镜面反射
    vec3 _reflectDir = reflect(_lightDir, _normal);
    float _specular = pow(max(dot(_reflectDir, _viewDir), 0.0f), myMaterial.m_shiness);
    vec3 _specularColor = _light.m_specular * _specular * vec3(texture(myMaterial.m_specular , outUV));

    return vec3(_ambientColor + _diffuseColor + _specularColor);
}

// 计算定向光下的环境光，漫反射，镜面反射
vec3 calculatePointLight(PointLight _light, vec3 _normal, vec3 _viewDir, vec3 _fragPos)
{
    // 入射光线
    vec3 _lightDir = normalize(_fragPos - _light.m_pos);

    // 环境光
    vec3 _ambientColor = _light.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));

    // 漫反射
    float _diffuse = max(dot(_normal, -_lightDir), 0.0f);
    vec3 _diffuseColor = _light.m_diffuse * _diffuse * vec3(texture(myMaterial.m_diffuse, outUV));

    // 镜面反射
    vec3 _reflectDir = reflect(_lightDir, _normal);
    float _specular = pow(max(dot(_reflectDir, _viewDir), 0.0f), myMaterial.m_shiness);
    vec3 _specularColor = _light.m_specular * _specular * vec3(texture(myMaterial.m_specular , outUV));

    // 衰减系数
    float _dist = length( _light.m_pos - _fragPos);
    float _attenuation = 1.0f / (_light.m_c + _light.m_l * _dist + _light.m_q * _dist * _dist);

    return vec3(_ambientColor + _diffuseColor + _specularColor) * _attenuation;
}

// 计算定向光下的环境光，漫反射，镜面反射
vec3 calculateSpotLight(SpotLight _light, vec3 _normal, vec3 _viewDir, vec3 _fragPos)
{
    // 入射光线
    vec3 _lightDir = normalize(_fragPos - _light.m_pos);
    vec3 _spotDir= normalize(_light.m_direction);
    float _cosTheta = dot(-_lightDir , -_spotDir);
    float _epsilon = _light.m_cutOff - _light.m_outCutOff;
    float _intensity = clamp((_cosTheta - _light.m_outCutOff) / _epsilon , 0.0f , 1.0f);

    // 环境光
    vec3 _ambientColor = _light.m_ambient * vec3(texture(myMaterial.m_diffuse, outUV));

    // 漫反射
    float _diffuse = max(dot(_normal, -_lightDir), 0.0f);
    vec3 _diffuseColor = _light.m_diffuse * _diffuse * vec3(texture(myMaterial.m_diffuse, outUV));

    // 镜面反射
    vec3 _reflectDir = reflect(_lightDir, _normal);
    float _specular = pow(max(dot(_reflectDir, _viewDir), 0.0f), myMaterial.m_shiness);
    vec3 _specularColor = _light.m_specular * _specular * vec3(texture(myMaterial.m_specular , outUV));

    // 衰减系数
    float _dist = length( _light.m_pos - _fragPos);
    float _attenuation = 1.0 / (_light.m_c + _light.m_l * _dist + _light.m_q * _dist * _dist);

    return vec3(_ambientColor + _diffuseColor + _specularColor) * _attenuation * _intensity;
}


void main()
{
    vec3 _viewDir = normalize(view_pos - outFragPos);
    vec3 _normal = normalize(outNormal);

    // 定向光
    vec3 result = calculateDirLight(_dirLight, _normal, _viewDir);

    // 点光源
    for (int i = 0; i < MAX_POINT_NUMBER; i++) {
        result += calculatePointLight(_pointLight[i], _normal, _viewDir, outFragPos);
    }

    // 聚光灯
    result += calculateSpotLight(_spotLight, _normal, _viewDir, outFragPos);

    FragColor = vec4(result, 1.0f);
}