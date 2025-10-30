"vim
set number
set numberwidth=1
set tabstop=4
set cursorline
set shiftwidth=4
set nowrap
nmap w :w<CR>
nmap q :q<CR>
colorscheme industry
autocmd BufRead,BufNewFile *.typ set wrap
autocmd BufRead,BufNewFile *.c,*.typ set shiftwidth=2
"plugin
call plug#begin('~/.config/nvim/plug')
Plug 'Yggdroot/indentLine'
Plug 'vim-airline/vim-airline'
Plug 'tomasiser/vim-code-dark'
Plug 'neoclide/coc.nvim',{'branch':'release'}
Plug 'chomosuke/typst-preview.nvim', {'tag': '*'}
call plug#end()
let g:airline_theme='codedark'
let g:airline_powerline_fonts=1
let g:indentLine_conceallevel=0
let g:indentLine_char_list=['┆','│']
"coc.nvim
hi CocFloating ctermfg=white ctermbg=darkblue
inoremap <expr> <CR> coc#pum#visible()?coc#_select_confirm():'<CR>'
"nvim
lua << EOF
require 'typst-preview'.setup {
	port = 8000,
	extra_args = {'--font-path=/system/fonts'},
	dependencies_bin = {
		['tinymist'] = 'tinymist',
		['websocat'] = 'websocat'
	}
}
EOF
