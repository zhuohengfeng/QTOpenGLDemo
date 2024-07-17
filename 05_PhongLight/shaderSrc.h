//
// Created by Rokid on 2024/7/17.
//

#ifndef QTOPENGLDEMO_SHADERSTR_H
#define QTOPENGLDEMO_SHADERSTR_H

const static char* cubeVertexShaderStr = R"(
    layout(location=0) in vec3 aPos;
    layout(location=1) in vec2 aUV;
    layout(location=2) in vec3 aNormal;

    out vec2 outUV;
    out vec3 outFragPos;
    out vec3 outNormal;

    uniform mat4 _projMatrix;
    uniform mat4 _viewMatrix;
    uniform mat4 _modelMatrix;

    void main()
    {
        gl_Position = _projMatrix * _viewMatrix * _modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
        outUV = aUV;
        outFragPos = vec3(_modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0));
        outNormal = mat3(transpose(inverse(_modelMatrix))) * aNormal;
    };
)";

const static char* cubeFragmentShaderStr = R"(
    out vec4 FragColor;

    in vec2 outUV;
    in vec3 outFragPos;
    in vec3 outNormal;

    struct Material
    {
        vec3 m_ambient;
        vec3 m_diffuse;
        vec3 m_specular;

        float m_shiness;
    };
    uniform Material myMaterial;

    struct Light
    {
        vec3 m_pos;
        vec3 m_ambient;
        vec3 m_diffuse;
        vec3 m_specular;
    };
    uniform Light myLight;

    uniform sampler2D ourTexture;
    // 观察点 相机位置
    uniform vec3 view_pos;

    void main()
    {
        //环境光
        vec3 _ambient = myLight.m_ambient * myMaterial.m_ambient;

        //漫反射
        vec3 _normal = normalize(outNormal);
        vec3 _lightDir = normalize(myLight.m_pos - outFragPos); // 光入射方向
        float _diff = max(dot(_normal , _lightDir) , 0.0f); // 入射光线和法线的夹角大小
        vec3 _diffuse = myLight.m_diffuse * _diff * myMaterial.m_diffuse;

        //镜面反射
        float _specular_strength = 0.5;
        vec3 _viewDir = normalize(view_pos - outFragPos); // 片元和眼镜的方向
        vec3 _reflectDir = reflect(-_lightDir , outNormal); // 反射方向

        float _spec = pow(max(dot(_viewDir , _reflectDir) , 0.0f) , myMaterial.m_shiness); // 计算反射强度

        vec3 _sepcular = myLight.m_specular * _spec * myMaterial.m_specular; // 镜面反射大小

        vec3 result = _ambient  + _diffuse + _sepcular;
        FragColor = texture(ourTexture , outUV) * vec4(result ,1.0f);
    };
)";

//
const static char* sunVertexShaderStr = R"(
    layout(location=0) in vec3 aPos;
    uniform mat4 _projMatrix;
    uniform mat4 _viewMatrix;
    uniform mat4 _modelMatrix;
    void main()
    {
        gl_Position = _projMatrix * _viewMatrix * _modelMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    };
)";

// 光源是白光
const static char* sunFragmentShaderStr = R"(
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 1.0f, 0.0f ,1.0f );
    };
)";



#endif //QTOPENGLDEMO_SHADERSTR_H
