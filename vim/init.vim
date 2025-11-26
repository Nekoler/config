"vim
set number
set numberwidth=1
set tabstop=4
set cursorline
set shiftwidth=4
set nowrap
nmap w :w<CR>
nmap q :q<CR>
colorscheme vim
autocmd BufRead,BufNewFile *.typ set wrap
autocmd BufRead,BufNewFile *.c,*.typ set shiftwidth=2
"plugin
call plug#begin('~/.config/nvim/plug')
Plug 'WuJunkai2004/indentLine'
Plug 'neoclide/coc.nvim',{'branch':'release'}
Plug 'chomosuke/typst-preview.nvim', {'tag': '*'}
call plug#end()
let g:indentLine_char_list=['|', '¦']
autocmd BufRead,BufNewFile *.typ,*.json let g:indentLine_conceallevel=0
"coc.nvim
hi CocFloating ctermfg=white ctermbg=darkblue
nmap <C-a> <Plug>(coc-codeaction-cursor)
imap <expr> <CR> coc#pum#visible()?coc#_select_confirm():'<CR>'
"nvim
lua << EOF
require 'typst-preview'.setup {
	port = 8000,
	extra_args = {'--font-path=/sdcard/Download/Fonts'},
	dependencies_bin = {
		['tinymist'] = 'tinymist',
		['websocat'] = 'websocat'
	}
}
EOF
