## 搭建编译环境
- Microsoft Visual Studio 2022
  1. 确保已安装VS 2022
  2. 启动 Visual Studio Installer
  4. 点 `Visual Studio Community 2022` 右侧的`修改`按钮，勾选以下内容
  - 工作负载:  
    - 「使用 C++ 的桌面开发」
  - 单个组件(以下内容开头带`*`号则可以任意版本号,默认选最新版本即可):
    - MSVC v143 - VS 2022 C++ x64/x86 生成工具(最新)
    - 适用于最新 v143 的 C++ ATL (x86 和 x64)  
    - vcpkg 包管理器
    - \* Windows 10 SDK  
    - \* Windows 11 SDK  
    - 适用于最新 v143 的 C++ MFC (x86 和 x64) 
  6. 应用更改
- Git™ for Windows（使用 vcpkg 期间请勿混用其他 Git）


## 编译源码（Building the source code）
源码含 2 个项目：
- MissionEditor：主程序
- MissionEditorPackLib：用 C 函数封装 XCC 对象，提供加载/打包逻辑

### 编译步骤（以尤里版为例）
1. 用 VS2022 打开 MissionEditor.sln
2. 顶部配置切换为「FinalAlertYRRelease」，输出自动指向 FinalAlert2YR.exe
3. F7 生成解决方案，产物在 dist/FinalAlert2 目录
4. F5 即可调试运行



## 更新三方库（Updating third-party libraries）
打开「Developer Command Prompt for VS2022」，cd 到 3rdParty\xcc，确认 PATH 含 git & vcpkg：

    git --version
    vcpkg x-update-baseline

> [!NOTE]
> xcc是已经内联修改了，不是按打patch的方式来的
<!-- > 仓库已内嵌裁剪版 XCC，补丁位于 `3rdParty\xcc\patch.<COMMIT_HASH>.diff`。如要升级 XCC，请替换文件后重新打补丁。 -->



## 手动打包发行（Creating a distribution）
目前本仓库已Github Action流水线，可自动完成编译工作
若改完代码仍然想手动发版，请先自行核对所有开源协议（含修改声明、版权追加等）。

脚本一键打包：

    cd scripts
    build_and_distribute.bat

生成文件（dist 目录）：
- FinalSun.zip → 泰伯利亚之日版
- FinalAlert2.zip → 红警 2 版
- FinalAlert2YR.zip → 尤里的复仇版
- MissionEditorSource.zip → 当前仓库源码（不含未提交变更）
- MissionEditorExternalSources.zip → 三方源码/二进制归档（可能含禁止再分发内容，仅留档）

**再强调：发版时务必自行满足本仓库及所有三方库的许可证义务，脚本不保证自动合规。**



## 目录结构速查（Directories）
- MissionEditor\data\shared：FS/FA2 共用数据
- MissionEditor\data\FinalAlert2：FA2 专有数据
- MissionEditor\data\FinalSun：FS 专有数据
- MissionEditor\PropertySheets：公共属性表，方便管理多配置
- dist：最终输出，含 exe、依赖 DLL 与 data，由 common.props 自动拷贝
