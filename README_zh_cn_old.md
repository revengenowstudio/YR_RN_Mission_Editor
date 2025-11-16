# FinalAlert（尤里的复仇）任务编辑器 RN 特别版

本仓库是《FinalAlert（YR）任务编辑器》的源代码，主要被 RN 团队二次定制。

官方主版本号已统一升至 2.0，以表明它们能在现代操作系统下正常运行。借此次更新，我们也顺手修了一批已知 Bug 并追加了一些实用功能。

---

## 正在进行的改动（Ongoing Changes）
- 强化 INI 解析，顺序与游戏原生保持一致（如 Animation 类），尚未处理重复注册
- 本地化：简体中文覆盖率 ≈ 90%
- 项目（Project）体系：一键切换工作空间（INI 定义、编辑器个性化、素材），单编辑器即可适配多个 MOD
- 全 D3D 渲染（* 低优先级，暂缓）
- 同步 FA2sp & HDM 版主要特性

---

## 已完成的修复与改进（Fixes & Changes）
- * 完整支持 x86_64 架构
- * 重写大部分 INI 逻辑，安全且高效
- * 引入 Google Test 单元测试
- 修复若干代码级兼容问题，可在 Win10/11 直接运行
- 应用图标升级至 256×256
- 支持鼠标中键/滚轮缩放地图
- 若安装在 Program Files 外，无需管理员权限（配置全部落到用户 AppData）
- 隧道系统彻底重做：
  + 提供隧道地形集
  + 可用工具直接编辑已有隧道端点
  + 支持曲线隧道
  + 支持单向隧道
- 泰伯利亚之日温带地图加入水晶/沼泽 LAT
- 玩家颜色自动读取 map 或 rules.ini
- 单位与建筑正确晕染玩家色
- Voxel 单位同步光影
- 小地图可自由缩放
- 撤销步数提升至 64 步
- 地图渲染性能优化
- 修复多处崩溃
- 附加炮塔显示修正
- SHP 炮塔 & Voxel 炮塔/炮管坐标修正
- 最大地图尺寸放开至 400×112（或 112×400）
- 非正方形地图小地图显示修复

---

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

---

## 编译源码（Building the source code）
源码含 2 个项目：
- MissionEditor：主程序
- MissionEditorPackLib：用 C 函数封装 XCC 对象，提供加载/打包逻辑

### 编译步骤（以尤里版为例）
1. 用 VS2022 打开 MissionEditor.sln
2. 顶部配置切换为「FinalAlertYRRelease」，输出自动指向 FinalAlert2YR.exe
3. F7 生成解决方案，产物在 dist/FinalAlert2 目录
4. F5 即可调试运行

---

## 更新三方库（Updating third-party libraries）
打开「Developer Command Prompt for VS2022」，cd 到 3rdParty\xcc，确认 PATH 含 git & vcpkg：

    git --version
    vcpkg x-update-baseline

> 注：仓库已内嵌裁剪版 XCC，补丁位于 `3rdParty\xcc\patch.<COMMIT_HASH>.diff`。如要升级 XCC，请替换文件后重新打补丁。

---

## 打包发行（Creating a distribution）
改完代码想发版，请先自行核对所有开源协议（含修改声明、版权追加等）。

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

---

## 目录结构速查（Directories）
- MissionEditor\data\shared：FS/FA2 共用数据
- MissionEditor\data\FinalAlert2：FA2 专有数据
- MissionEditor\data\FinalSun：FS 专有数据
- MissionEditor\PropertySheets：公共属性表，方便管理多配置
- dist：最终输出，含 exe、依赖 DLL 与 data，由 common.props 自动拷贝

---

## 后续维护（Contributions）
本仓库已归档，官方不再维护。如需修复或加功能，请直接 Fork。

---

## 源码年代（Source）
主体代码写于 1999-2001 年，未用智能指针、RAII 或硬件加速。我们已开启 C++20 并重构关键模块，方便后续迭代。

---

## 作者与致谢（Authors & Thanks）
- Electronic Arts Inc.
- Matthias Wagner  
  - FinalSun & FinalAlert2 原生作者  
  - Bug 修复、功能更新、构建系统升级
- Olaf van der Spek → XCC 库
- Luke "CCHyper" Feenan  
  - 额外编码、开源流程梳理、新图标与素材

特别感谢 EA 官方批准开源。

---

## 法律声明（Legal）
《命令与征服：泰伯利亚之日》《命令与征服：红色警戒 2》《命令与征服：尤里的复仇》版权归属 Westwood Studios，Westwood 为 Electronic Arts 商标。  
Microsoft、DirectX、Visual C++、Visual Studio、Windows 为微软集团商标。  
Git 及 Git Logo 为 Software Freedom Conservancy 在美国或其他国家/地区的商标或注册商标。

---

## 开源协议（License）
除非文件内另有声明，本仓库源码采用 **GNU General Public License v3**。详见 LICENSE 文件。  
3rdParty 内各库可能适用其他协议，请分别查阅其 LICENSE/COPYING。