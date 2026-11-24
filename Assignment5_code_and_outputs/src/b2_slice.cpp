#include <iostream>
#include <stdexcept>
struct ClaimError : std::runtime_error { using std::runtime_error::runtime_error; };
void viaRethrow() { try { throw ClaimError("exceeds cover"); } catch (const std::exception& e) { throw; } }
void viaThrowE()  { try { throw ClaimError("exceeds cover"); } catch (const std::exception& e) { throw e; } }
int main()
{
 try { viaRethrow(); }
 catch (const ClaimError& e) { std::cout << "throw;   -> caught as ClaimError: " << e.what() << '\n'; }
 catch (const std::exception& e) { std::cout << "throw;   -> caught as std::exception: " << e.what() << '\n'; }
 try { viaThrowE(); }
 catch (const ClaimError& e) { std::cout << "throw e; -> caught as ClaimError: " << e.what() << '\n'; }
 catch (const std::exception& e) { std::cout << "throw e; -> caught as std::exception: " << e.what() << '\n'; }
}
