#include "ShaderFactory.h"


using namespace Pro;
using namespace Graphics;
 
void ShaderFactory::enable(Shader_Module module){
	shader_segments.push_back(module);
}
 
Shader ShaderFactory::generate() const{
	// Include all files into a single string
	 
	Buffer shader(shader_segments.size() * 1000);  
	BufferReader reader(&shader);
	BufferWriter writer(&shader);
	
	for (unsigned x = 0; x < shader_segments.size(); ++x)
		writer.write_array<char>(shader_modules[shader_segments[x]],
							sizeof(shader_modules[shader_segments[x]]));

	// Preprocessing 

	while (reader.hasNext()) {
		Buffer line = std::move(reader.read_delim('\n', false));
		if (*line.data<char>() == '~') {
			BufferReader line_r(&line);
			if (line_r.contains("include", 7)) {

			}
		}
	}

	// Create a main function from the order of .enable calls


}