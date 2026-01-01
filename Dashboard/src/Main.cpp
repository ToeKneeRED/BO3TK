#include "Runner.h"

int main(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ const LPWSTR lpCmdLine, _In_ int)
{
    Runner* runner = new Runner();

    return runner->Start(lpCmdLine);
}