writing
=======

writing Chinese

###已实现
- 弹出窗口类PopLayer 设计未全部完成
- 启动界面Layer，WallSceneLayer，lianxi Layer，简单界面布局
- 和Lua交互实现： 1. 从Lua文件获取String 2. 从Lua文件获取Table 3.调用Lua全局函数 4. 传参数给Lua函数 5. Lua函数调用static C++函数
- 字符编码转换DataTool类

###设计实现需修改地方
####界面设计和事件响应
在练习界面总觉得应该将布局和事件响应分开，分开两个Layer处理，但是鉴于不清楚multiLayer间交互，不清楚实现过程
似乎应该将田字格单独包装成为CCNode自定义类，单独在此类中处理田字格正字及手写字处理逻辑
同理手写字动画效果也应该在CCNode自定义类中实现。
####Lua交互
Lua交互还有一些问题，应该将函数更进一步的封装成类，虽然现在能够使用的类已经有了HclcData(LuaData),CLuaScriptReader，但是整体条理不是很清楚。




###麻烦之处
- 由于我们的xml正字信息坐标系问题，引入进来之后坐标变换繁多。在正字笔画旋转和缩放操作时无法兼顾
- 动画的处理过程
- 字部件笔画三级的处理过程，目前只是针对简单字，如果涉及到部件，不清楚是否需要改动某些代码。
