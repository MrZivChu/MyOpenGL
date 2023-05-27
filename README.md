# MyOpenGL
创建对象一般四步骤：
					1、生成 Gen  
					2、绑定 Bind(id)  
					3、设置属性
					4、解绑 Bind(0)
使用的时候：
					绑定 Bind(id)  [Shader除外，使用：glUseProgram(id)]
销毁对象：
					删除 Delete(id)