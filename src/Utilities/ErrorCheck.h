namespace Pro{
	namespace Util{
		//! Used to check the return values of functions
		//! and reduce the amount of visible branches in source.
		template<typename T = unsigned>
		class ErrorCheck{
			unsigned short had_error_;
			T success_code_;

		public:
			ErrorCheck(unsigned success_code){
				this->success_code_ = success_code;
			}

			bool HadError(){
				return had_error_ != 0;
			}

			unsigned char ErrorCount(){
				return had_error_;
			}
			 
			ErrorCheck& operator=(const T& code){
				if (code != success_code_)
					++had_error_;
				return *this;
			}
		};
	}
}