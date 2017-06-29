/*************************************************************************
 Protheus Source File.
 Copyright (C), Protheus Studios, 2013-2016.
 -------------------------------------------------------------------------
 
 Description:
 
 -------------------------------------------------------------------------
 History:
 - 6:5:2015 Waring J.
 
 *************************************************************************/


namespace Pro{
	namespace Util{
		//! Used to check the return values of functions
		//! and reduce the amount of visible branches in source.
		template<typename T = unsigned>
		class ErrorCheck{
			unsigned short had_error_;
			T success_code_;

		public:
			//! success_code is used to determine if a function executed correctly
			//! if returned value doesn't match the success_code then an error occured
			ErrorCheck(const T& success_code){
				this->success_code_ = success_code;
			}

			//! Returns true if a non @success_code was passed
			bool HadError(){
				return had_error_ != 0;
			}

			//! Returns the count of errors
			unsigned short ErrorCount() const{
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