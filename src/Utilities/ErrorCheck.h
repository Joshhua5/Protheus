namespace Pro{
	namespace Util{
		class ErrorCheck{
			unsigned char had_error;
			unsigned success_code;

		public:
			ErrorCheck(unsigned success_code){
				this->success_code = success_code;
			}

			bool hadError(){
				return had_error != 0;
			}

			unsigned char error_count(){
				return had_error;
			}

			template<typename T>
			ErrorCheck& operator=(const T& code){
				if (code != success_code)
					++had_error;
				return *this;
			}
		};
	}
}