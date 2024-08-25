```js quickadd
const title = await this.quickAddApi.wideInputPrompt("👀文章标题");
const clas = await this.quickAddApi.wideInputPrompt("✨文章分类");
const tags = await this.quickAddApi.wideInputPrompt("🔖标签");

// YAML 头 
const yamlHeader = `---
title: 【${clas}】${title} 
tags: 
 - ${tags}
aliases: 
 - ${title}
---`;
return yamlHeader;
```

> 概述：

<!--more-->

## 