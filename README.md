# FinalAlert（尤里的复仇）地图编辑器 RN 特别版

本项目是《FinalAlert（YR）地图编辑器》 RN 团队二次定制版本（下文以"FA2 RN版"、"RN特别版地编"或"本地编"代称），虽然名为"RN特别版"，但是也并非只支持RN这一个Mod。本地编在EA开源版地编的基础上，兼顾了软件开发维护者的开发体验和地图编辑器成品用户体验，目的在于打造一个易于维护的，功能友好的工具软件。

官方主版本号已统一升至 2.0，以表明它们能在现代操作系统下正常运行。


## 下载和安装
1. 访问本仓库[Releases页面](https://github.com/revengenowstudio/YR_RN_Mission_Editor/releases)即可下载最新版本
2. 确保已经安装[VC++14 运行时](https://learn.microsoft.com/zh-cn/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-supported-redistributable-version)
3. 解压从Release下载的zip压缩包，运行`FinalAlert2YR.exe`，首次运行需要指定ra2或者Mod资源的位置，运行程序后，在弹出的`基本选项`窗口中的`Language`选择`简体中文`，奠基`Red Alert 2 EXE`右侧的按钮，选择`ra2.mix`即可

  ![1763296772166](image/README/1763296772166.png)

4. 本地编的`FinalAlert.ini`并不保存在exe所在目录，而是位于 : `%LOCALAPPDATA%/FinalAlert 2/FinalAlert.ini`(此路径粘贴到资源管理器的路径栏即可打开)

> [!NOTE]
> 即便现阶段本地编功能不完整,我们仍不推荐手动修改`FinalAlert.ini`，未来本地编会有独立的窗口来便捷控制这个文件的配置。


## 本项目相较于FA2SP的开发优势
- 基于EA开源版FA2代码库基础上进行开发，源码可见，无需像FA2SP那样需要依赖`syringe`注入器和高超的逆向知识才能开发新功能和修复FA2的众多bug
- 在Zero-Fanker的不懈努力下，本项目引入了现代的单元测试，重构了大部分可读性堪忧的旧代码并重构位64位程序，等现代的开发流程和现代的代码工具，这也意味着在此仓库的开发体验会比FA2SP优秀许多
- 得益与现代编译器的优化和Zero-Fanker的相关代码重构方面的努力，本地编有望获得比其它FA2原版扩展程序更好的性能和更稳定的体验。


## 正在进行的改动（Ongoing Changes）
- 强化 INI 解析，顺序与游戏原生保持一致（如 Animation 类），尚未处理重复注册
- 本地化：简体中文覆盖率 ≈ 95%
- 项目（Project）体系：一键切换工作空间（INI 定义、编辑器个性化、素材），单编辑器即可适配多个 Mod
- 全 D3D 渲染（* 低优先级，暂缓）
- 支持读取XML格式的CSF内容
- 同步 FA2SP & HDM 版主要特性


## 已完成的修复与改进（Fixes & Changes）

### 功能改进
1. 主视角支持鼠标滚轮自由缩放（by @handama）
2. 撤销步数上限提升至 64 步；可一次性撤销“长按连续放置”的覆盖物，大地形撤销不再残留（by @handama）
3. 隧道系统彻底重做：提供隧道地形集，可视化编辑端点，支持曲线/单向隧道(by @Zero-Fanker)
4. 最大地图尺寸放开至 400×112 或 112×400
5. 泰伯利亚之日温带地图新增水晶/沼泽 LAT(by @Zero-Fanker)
6. 自动海岸重写：不会在非海岸区误创海岸，可正确摆放特殊海岸（by @handama）
7. 引入 HDM 版 CSF 查看器窗口，支持双击应用 CSF 内容(by @Zero-Fanker)
8. 地图保存时支持“最小玩家数”设置(by @Zero-Fanker)
9. 工作区功能：读取地图时同步加载同目录 FinalAlertProject.ini，重新挂载指定游戏资源(by @Zero-Fanker)
10. 玩家颜色自动读取 map 或 rules.ini；单位与建筑正确晕染玩家色(by @Zero-Fanker)
11. Voxel 单位光影同步；SHP 炮塔 & Voxel 炮塔/炮管坐标修正；附加炮塔显示修正(by @Zero-Fanker)

### Bug 修复
1. 修复多次重新读取地图导致内存溢出的问题(by @Zero-Fanker)
2. 修复调整窗体大小或渲染区域超出屏幕时极易崩溃的问题(by @Zero-Fanker)
3. 修复抬升地图边缘时崩溃的问题(by @handama，@Zero-Fanker)
4. 修复步兵单元格显示位置与游戏内不一致
5. 修复触发事件 23 不显示小队（by @handama）
6. 修复小队脚本缺少脚本/路径点参数的问题(by @Zero-Fanker)
7. 修复部分子窗口功能不正确问题的问题(by @Zero-Fanker)
8. 修复物品栏取消选择后焦点自动跳回顶端（by @handama）
9. 修复非正方形地图小地图显示错误的问题(by @Zero-Fanker)
10. 修复Win10/11 下若干代码级兼容问题导致无法直接运行的问题(by @Zero-Fanker)

### 性能优化
1. 地图渲染整体性能优化(by @Zero-Fanker)
2. 使用 ddraw7 接口替换老旧 ddraw4，提升绘制效率(by @Zero-Fanker)
3. 重写大部分 INI 读写逻辑，降低内存占用并加快解析速度(by @Zero-Fanker)
4. 撤销系统算法优化，减少卡顿与内存峰值(by @Zero-Fanker，by @handama)
5. 引入 Google Test 单元测试，持续锁定性能回归(by @Zero-Fanker)
6. 本地编彻底改造成为64位应用程序(by @Zero-Fanker)


## 使用指南和开发指南
- 本地编详细使用指南参见 : [使用指南](./docs/UserGuide/README.md)
- 开发指南参见 : [开发指南](./docs/DevelopGuide/README.md)


## 作者与致谢（Authors & Thanks）
- Electronic Arts Inc.
- Matthias Wagner  
  - FinalSun & FinalAlert2 原生作者  
  - Bug 修复、功能更新、构建系统升级
- Olaf van der Spek → XCC 库
- Luke "CCHyper" Feenan  
  - 额外编码、开源流程梳理、新图标与素材
- @Zero-Fanker
  - RN特别版地编核心开发，主持整个RN特别版地编的功能重构/开发/维护
- @handama
  - FA2SP HDM Edition 主创，基于 FA2SP 的逆向成果接手FA2SP的功能开发，给RN特别版地编提供了非常多idea和代码支持

特别感谢 EA 官方批准开源。


## 法律声明 
《命令与征服：泰伯利亚之日》《命令与征服：红色警戒 2》《命令与征服：尤里的复仇》版权归属 Westwood Studios，Westwood 为 Electronic Arts 商标。  
Microsoft、DirectX、Visual C++、Visual Studio、Windows 为微软集团商标。  
Git 及 Git Logo 为 Software Freedom Conservancy 在美国或其他国家/地区的商标或注册商标。


## 开源协议
除非文件内另有声明，本仓库源码采用 **GNU General Public License v3**。详见 LICENSE 文件。  
3rdParty 内各库可能适用其他协议，请分别查阅其 LICENSE/COPYING。