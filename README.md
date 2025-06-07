# BO3TK

 another side project :)

 ## Requirements
 - [Visual Studio 2022](https://visualstudio.microsoft.com/downloads/)
 - [xmake](https://xmake.io/#/)

 ## Build
 - run `build.bat`

   or
 - Navigate to the root `BO3TK` folder
 - `xmake` (can also optionally use the `-y` flag to skip package install confirmation prompt)
 
 ## Generate Visual Studio solution
 - run `build.bat`

   or
 - Navigate to the root `BO3TK` folder
 - `xmake project -k vsxmake` (.sln inside `vsxmake2022` directory)

 ## Options
 - `game`: Path to game exe (ex. `xmake f --game="C:\Path\To\Game\BlackOps3.exe"`)
 - `dll`: Path to dll to inject (ex. `xmake f --dll="C:\Path\To\Dll\BO3TK.dll`)
  > [!NOTE]
  > You can combine the two to set them both at once (ex. `xmake f --game="C:\Path\To\Game\BlackOps3.exe" --dll="C:\Path\To\Dll\BO3TK.dll`)
