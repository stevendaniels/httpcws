## Introduction in English ##

> HTTPCWS is a open-source Chinese Word Segmentation System Based on the HTTP protocol, using ICTCLAS Chinese word segmentation algorithms.

> ICTCLAS is a Chinese lexical analysis system, which is able to make Chinese word segmentation, Part-Of-Speech tagging, word sense disambiguation and named entity recognition. The detailed linguistic information provided by ICTCLAS increases the accuracy and depth of any application related to Chinese language, such as machine translation, retrieval, filtering, text mining and many others.

## 发布版本： ##
**httpcws 1.0.0** （最新版本：2009-08-10发布）

> 安装使用手册：http://blog.s135.com/httpcws_v100/

> 下载地址（32位版）：http://httpcws.googlecode.com/files/httpcws-1.0.0-i386-bin.tar.gz

> 下载地址（64位版）：http://httpcws.googlecode.com/files/httpcws-1.0.0-x86_64-bin.tar.gz

> 中文分词在线演示：http://blog.s135.com/demo/httpcws/

> PHP演示程序下载：http://blog.s135.com/demo/httpcws/httpcws-php-demo.zip


---


## httpcws 中文简介 ##
#### 1、什么是 httpcws ？ ####
> HTTPCWS 是一款基于HTTP协议的开源中文分词系统，目前仅支持Linux系统。HTTPCWS 使用“ICTCLAS 3.0 2009共享版中文分词算法”的API进行分词处理，得出分词结果。

> [ICTCLAS](http://ictclas.org/sub_1_2.html)（Institute of Computing Technology, Chinese Lexical Analysis System）是中国科学院计算技术研究所在多年研究工作积累的基础上，基于多层隐马模型研制出的汉语词法分析系统，主要功能包括中文分词；词性标注；命名实体识别；新词识别；同时支持用户词典。ICTCLAS经过五年精心打造，内核升级6次，目前已经升级到了ICTCLAS3.0，分词精度98.45%，各种词典数据压缩后不到3M。ICTCLAS在国内973专家组组织的评测中活动获得了第一名，在第一届国际中文处理研究机构SigHan组织的评测中都获得了多项第一名，是当前世界上最好的汉语词法分析器。

> ICTCLAS 3.0 商业版是收费的，而免费提供的 ICTCLAS 3.0 共享版不开源，词库是根据人民日报一个月的语料得出的，很多词语不存在。所以本人补充的一个19万条词语的自定义词库，对ICTCLAS分词结果进行合并处理，输出最终分词结果。

> 由于 ICTCLAS 3.0 2009 共享版只支持GBK编码，因此，如果是UTF-8编码的字符串，可以先用iconv函数转换成GBK编码，再用httpcws进行分词处理，最后转换回UTF-8编码。

> HTTPCWS 软件自身（包括httpcws.cpp源文件、dict/httpcws\_dict.txt自定义词库）采用NewBSD开源协议，可以自由修改。HTTPCWS 使用的 ICTCLAS 共享版 API 及 dict/Data/ 目录内的语料库，版权及著作权归中国科学院计算技术研究所、ictclas.org所有，使用需遵循其相关协议。

#### 2、httpcws 中文分词在线演示 ####
> 演示网址：http://blog.s135.com/demo/httpcws/

#### 3、httpcws 中文分词下载安装 ####
> 32位版：
```
cd /usr/local/
wget http://httpcws.googlecode.com/files/httpcws-1.0.0-i386-bin.tar.gz
tar zxvf httpcws-1.0.0-i386-bin.tar.gz
rm -f httpcws-1.0.0-i386-bin.tar.gz
cd httpcws-1.0.0-i386-bin/
ulimit -SHn 65535
/usr/local/httpcws-1.0.0-i386-bin/httpcws -d -x /usr/local/httpcws-1.0.0-i386-bin/dict/
```

> 64位版：
```
cd /usr/local/
wget http://httpcws.googlecode.com/files/httpcws-1.0.0-x86_64-bin.tar.gz
tar zxvf httpcws-1.0.0-x86_64-bin.tar.gz
rm -f httpcws-1.0.0-x86_64-bin.tar.gz
cd httpcws-1.0.0-x86_64-bin/
ulimit -SHn 65535
/usr/local/httpcws-1.0.0-x86_64-bin/httpcws -d -x /usr/local/httpcws-1.0.0-x86_64-bin/dict/
```

> 命令行启动参数：

> ![http://s135.com/attachment/200908/httpcws-help.png](http://s135.com/attachment/200908/httpcws-help.png)

#### 4、httpcws 使用方法 ####
> GET方法（文本长度受URL的长度限制，需要分词的文本为GBK编码，最好采用urlencode对文本进行编码）：
```
http://192.168.8.42:1985/?w=有人的地方就有江湖
http://192.168.8.42:1985/?w=%D3%D0%C8%CB%B5%C4%B5%D8%B7%BD%BE%CD%D3%D0%BD%AD%BA%FE
```

> POST方法（文本长度无限制，适用于大文本分词，需要分词的文本为GBK编码，最好采用urlencode对文本进行编码）：
```
curl -d "有人的地方就有江湖" http://192.168.8.42:1985
curl -d "%D3%D0%C8%CB%B5%C4%B5%D8%B7%BD%BE%CD%D3%D0%BD%AD%BA%FE" http://192.168.8.42:1985
```

> PHP 调用 HTTPCWS 示例见：http://blog.s135.com/httpcws_v100/

#### 5、httpcws 分词速度及用途 ####

> 局域网内 HTTPCWS 接口中文分词平均处理速度（Wait时间）：0.001秒，每秒可处理5000～20000次请求。

> ![http://s135.com/attachment/200908/httpcws-bc.png](http://s135.com/attachment/200908/httpcws-bc.png)

> HTTPCWS 属于《[亿级数据的高并发通用搜索引擎架构设计](http://blog.s135.com/post/385.htm)》的一部分，用作“搜索查询接口”的关键字分词处理。在此架构中，Sphinx索引引擎对于CJK（中日韩）语言支持一元切分，假设【反恐行动是国产主视角射击网络游戏】这段文字，Sphinx会将其切成【反 恐 行 动 是 国 产 主 视 角 射 击 网 络 游 戏】，然后对每个字建立反向索引。如果用这句话中包含的字组成一个不存在的词语，例如【恐动】，也会被搜索到，所以搜索时，需要加引号，例如搜索【"反恐行动"】，就能完全匹配连在一起的四个字，不连续的【"恐动"】就不会被搜索到。但是，这样还有一个问题，搜索【"反恐行动游戏"】或【"国产网络游戏"】就会搜索不到。所以，我在搜索层写了个PHP中文分词扩展，搜索“反恐行动游戏”、“国产网络游戏”，会被httpcws中文分词函数分别切分为“反恐行动 游戏”、“国产 网络游戏”，这时候，用PHP函数给以空格分隔的词语加上引号，去搜索【"反恐行动" "游戏"】或【"国产" "网络游戏"】，就能搜索到这条记录了。由于httpcws位于搜索层，中文分词词库发生增、删、改，只需重启httpcws进程即可，无需重建搜索索引。

> 根据上述情况，对于那些采用二元交叉切分的搜索引擎，httpcws用在前端搜索层对用户输入的搜索关键字、短语进行分词处理，同样适合。httpcws开发的目的正在于此，对于短句、小文本中文分词切分，速度非常之快。

#### 6、自定义词库 ####
> 修改dict/httpcws\_dict.txt文件，可以自由增加自己需要的词语。重启httpcws即可生效。