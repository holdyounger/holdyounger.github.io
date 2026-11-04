(function($){
    $(function(){
        var $toc = $('.toc-article');
        if (!$toc.length) return;

        var storageKey = 'wikitten.toc.collapsed';
        var collapsed = localStorage.getItem(storageKey) === '1';

        function applyState(){
            $toc.toggleClass('collapsed', collapsed);
            // ensure content visibility matches state
            $toc.find('#toc-content').toggle(!collapsed);
            $toc.find('.toc-title').toggle(!collapsed);
        }

        applyState();

        $toc.on('click', '.toc-toggle', function(e){
            e.stopPropagation();
            collapsed = !collapsed;
            localStorage.setItem(storageKey, collapsed ? '1' : '0');
            applyState();
        });

        // 点击页面任意处时若处于折叠状态则自动展开（可选）
        /*
        $(document).on('click', function(e){
            if (collapsed && !$(e.target).closest('.toc-article').length) {
                collapsed = false;
                localStorage.setItem(storageKey, '0');
                applyState();
            }
        });
        */
    });
})(jQuery);
