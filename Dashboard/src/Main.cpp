#include "Runner.h"

int main(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ const LPWSTR lpCmdLine, _In_ int)
{
    const std::unique_ptr<Runner> cRunner = std::make_unique<Runner>();

    return cRunner->Start(lpCmdLine);
}