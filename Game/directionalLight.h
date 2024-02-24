#include <glm/glm.hpp>


class DirectionalLight 
{
public:
	DirectionalLight(glm::vec3 direction);
	void setIntensity(glm::vec4 intensity);

	



protected:
private:
	glm::vec3 direction;
	glm::vec4 intensity;

};

