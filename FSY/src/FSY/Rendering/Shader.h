#include "../Maths/Maths.h"
#include "../Core/Core.h"

#include <string>

namespace FSY {
	
    class FSY_API Shader
    {
    public:
        // the program ID
        unsigned int ID;
        Shader(const char* vertexPath, const char* fragmentPath);
        Shader();
        ~Shader();

        // use/activate the shader
        void Use();
        //delete the shader
        void Delete();

        // utility uniform functions
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setColorValues3(const std::string& name, float r, float g, float b) const;
        void setColorValues4(const std::string& name, float r, float g, float b, float a) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

        //Attributes that should be on every Shader
        Vector3f diffuse = {1.0f, 1.0f, 1.0f};
        Vector3f specular = { 0.2f, 0.2f, 0.2f };
        float shininess = 32.0f;
    };

}