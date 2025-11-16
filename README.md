# FinalAlert（尤里的复仇）任务编辑器 RN 特别版

本项目是《FinalAlert（YR）任务编辑器》 RN 团队二次定制版本，虽然名为"RN特别版"，但是也并非只支持RN这一个mod。我们在EA开源版地编的基础上，添加了一系列对地图开发者和mod开发者友好的功能。

我们的目标是使FA2的功能和性能和现代IDE靠拢，尽可能减轻地图开发者和mod开发者的地图开发工作负担

除此之外，本仓库已经大规模重构了EA版FA2的代码，引入了了C++20标准和单元测试等能提高地编开发效率和代码质量的开发工具，我们也为想给地编添加新功能的各路开发者提供了便利

官方主版本号已统一升至 2.0，以表明它们能在现代操作系统下正常运行。



## 下载和安装
1. 访问本仓库[Releases页面](https://github.com/revengenowstudio/YR_RN_Mission_Editor/releases)即可下载最新版本
2. 确保已经安装[VC++14 运行时](https://learn.microsoft.com/zh-cn/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-supported-redistributable-version)
3. 解压从Release下载的zip压缩包，运行`FinalAlert2YR.exe`，首次运行需要指定ra2或者mod资源的位置，运行程序后，在弹出的`基本选项`窗口中的`Language`选择`简体中文`，奠基`Red Alert 2 EXE`右侧的按钮，选择`ra2.mix`即可

  ![1763296772166](image/README/1763296772166.png)

4. 若需要修改`FinalAlert.ini`，FA2 RN版的`FinalAlert.ini`并不保存在exe所在目录下，二手位于用户目录 : `%LOCALAPPDATA%/FinalAlert 2/FinalAlert.ini`(贴到资源管理器的路径栏即可打开)


## 本项目相较于FA2SP的开发优势
- 基于EA开源版FA2代码库基础上进行开发，源码可见，无需像FA2SP那样需要依赖`syringe`注入器和高超的逆向知识才能开发新功能和修复FA2的众多bug
- 在ZeroFanker的不谢努力下，本项目引入了现代的单元测试，重构了大部分可读性堪忧的旧代码并重构位64位程序，等现代的开发流程和现代的代码工具，这也意味着在此仓库的开发体验会比FA2SP优秀许多
- 得益与现代编译器的优化和ZeroFanker的相关代码重构方面的努力，本项目的运行速度能比FA2SP更优

## 正在进行的改动（Ongoing Changes）
- 强化 INI 解析，顺序与游戏原生保持一致（如 Animation 类），尚未处理重复注册
- 本地化：简体中文覆盖率 ≈ 95%
- 项目（Project）体系：一键切换工作空间（INI 定义、编辑器个性化、素材），单编辑器即可适配多个 MOD
- 全 D3D 渲染（* 低优先级，暂缓）
- 支持读取XML格式的CSF内容
- 同步 FA2sp & HDM 版主要特性

## 已完成的修复与改进（Fixes & Changes）

### 功能改进
1. 主视角支持鼠标滚轮自由缩放（handama）
2. 撤销步数上限提升至 64 步；可一次性撤销“长按连续放置”的覆盖物，大地形撤销不再残留（handama）
3. 隧道系统彻底重做：提供隧道地形集，可视化编辑端点，支持曲线/单向隧道(ZeroFanker)
4. 最大地图尺寸放开至 400×112 或 112×400
5. 泰伯利亚之日温带地图新增水晶/沼泽 LAT(ZeroFanker)
6. 自动海岸重写：不会在非海岸区误创海岸，可正确摆放特殊海岸（handama）
7. 引入 HDM 版 CSF 查看器窗口，支持双击应用 CSF 内容(ZeroFanker)
8. 地图保存时支持“最小玩家数”设置(ZeroFanker)
9. 工作区功能：读取地图时同步加载同目录 FinalAlertProject.ini，重新挂载指定游戏资源(ZeroFanker)
10. 玩家颜色自动读取 map 或 rules.ini；单位与建筑正确晕染玩家色(ZeroFanker)
11. Voxel 单位光影同步；SHP 炮塔 & Voxel 炮塔/炮管坐标修正；附加炮塔显示修正(ZeroFanker)

### Bug 修复
1. 修复多次重新读取地图导致内存溢出的问题(ZeroFanker)
2. 修复调整窗体大小或渲染区域超出屏幕时极易崩溃的问题(ZeroFanker)
3. 修复抬升地图边缘时崩溃的问题(handama， ZeroFanker)
4. 修复步兵单元格显示位置与游戏内不一致
5. 修复触发事件 23 不显示小队（handama）
6. 修复小队脚本缺少脚本/路径点参数的问题(ZeroFanker)
7. 修复部分子窗口功能不正确问题的问题(ZeroFanker)
8. 修复物品栏取消选择后焦点自动跳回顶端（handama）
9. 修复非正方形地图小地图显示错误的问题(ZeroFanker)
10. 修复Win10/11 下若干代码级兼容问题导致无法直接运行的问题(ZeroFanker)

### 性能优化
1. 地图渲染整体性能优化(ZeroFanker)
2. 使用 ddraw7 接口替换老旧 ddraw4，提升绘制效率(ZeroFanker)
3. 重写大部分 INI 读写逻辑，降低内存占用并加快解析速度(ZeroFanker)
4. 撤销系统算法优化，减少卡顿与内存峰值(ZeroFanker，handama)
5. 引入 Google Test 单元测试，持续锁定性能回归(ZeroFanker)
6. 正式提供 64 位构建（完整 x86_64 支持）(ZeroFanker)


## 搭建编译环境（Install build requirements）
- Microsoft Visual Studio 2022
  1. 安装 VS2022（如已装可跳过）
  2. 重启 Visual Studio Installer
  3. 打齐所有更新
  4. 点「修改」
  5. 工作负载勾选「使用 C++ 的桌面开发」，右侧细节务必再勾：
     - MSVC v143 - VS2022 C++ x64/x86 生成工具  
     - 适用于 v143 的 C++ ATL (x86 & x64)  
     - vcpkg 包管理器（若看不到请先升级 VS）  
     - 最新 Windows 10 SDK  
     - 最新 Windows 11 SDK  
     - 适用于 v143 的 C++ MFC (x86 & x64)  
     - 其余保持默认
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

> 注：仓库已内嵌裁剪版 XCC，补丁位于 `3rdParty\xcc\patch.<COMMIT_HASH>.diff`。如要升级 XCC，请替换文件后重新打补丁。



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


## 作者与致谢（Authors & Thanks）
- Electronic Arts Inc.
- Matthias Wagner  
  - FinalSun & FinalAlert2 原生作者  
  - Bug 修复、功能更新、构建系统升级
- Olaf van der Spek → XCC 库
- Luke "CCHyper" Feenan  
  - 额外编码、开源流程梳理、新图标与素材

- @ZeroFanker
  - RN特别版地编核心开发，主持整个RN特别版地编的功能重构/开发/维护
- @handama
  - FA2SP HDM Edition 主创，基于 FA2SP 的逆向成果接手FA2SP的功能开发，给RN特别版地编提供了非常多idea和代码支持

特别感谢 EA 官方批准开源。


## 法律声明（Legal）
《命令与征服：泰伯利亚之日》《命令与征服：红色警戒 2》《命令与征服：尤里的复仇》版权归属 Westwood Studios，Westwood 为 Electronic Arts 商标。  
Microsoft、DirectX、Visual C++、Visual Studio、Windows 为微软集团商标。  
Git 及 Git Logo 为 Software Freedom Conservancy 在美国或其他国家/地区的商标或注册商标。



## 开源协议（License）
除非文件内另有声明，本仓库源码采用 **GNU General Public License v3**。详见 LICENSE 文件。  
3rdParty 内各库可能适用其他协议，请分别查阅其 LICENSE/COPYING。