one stage 速度快
two stage rpn 候选框
maskRcnn 网络结构
yolov1 提取两种候选框
448x448x3的网络 7x7x1024特征图 
全连接层 4096x1 7x7x30（ xywhs xywh + 20类别）
S*S*(B*5+C) 损失函数：位置误差+置信度误差+分类误差
