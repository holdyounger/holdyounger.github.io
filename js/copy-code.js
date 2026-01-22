(function($){
    $(function(){
        $('.article-entry pre').each(function(){
            var $pre = $(this);
            if ($pre.parent().hasClass('code-wrapper')) return;
            if ($pre.parent().hasClass('gutter')) return;
            $pre.wrap('<div class="code-wrapper"></div>');
            var $wrapper = $pre.parent().parent().parent().parent().parent();
            var $btn = $('<button class="code-copy-btn" type="button" title="复制代码" style="float:right;">复制</button>');
            $wrapper.before($btn);

            $btn.on('click', function(){
                var text = $pre.text();
                if (navigator.clipboard && navigator.clipboard.writeText) {
                    navigator.clipboard.writeText(text).then(function(){
                        showTip($btn, '已复制');
                    }).catch(function(){
                        fallbackCopy(text, $btn);
                    });
                } else {
                    fallbackCopy(text, $btn);
                }
            });
        });

        function fallbackCopy(text, $btn) {
            var textarea = document.createElement('textarea');
            textarea.value = text;
            textarea.style.position = 'fixed';
            textarea.style.left = '-9999px';
            document.body.appendChild(textarea);
            textarea.select();
            try {
                var ok = document.execCommand('copy');
                showTip($btn, ok ? '🥰 已复制' : '😐 复制失败');
            } catch (e) {
                showTip($btn, '复制失败');
            }
            document.body.removeChild(textarea);
        }

        function showTip($btn, msg) {
            var $tip = $('<span class="code-copy-tip">'+msg+'</span>');
            $btn.after($tip);
            setTimeout(function(){
                $tip.fadeOut(200, function(){ $(this).remove(); });
            }, 1200);
        }
    });
})(jQuery);
