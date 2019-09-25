syntax on

set tabstop=4 
set shiftwidth=4 
set expandtab
set autoindent

set colorcolumn=80 
highlight ColorColumn ctermbg=darkgray

augroup project   
    autocmd!   
    autocmd BufRead,BufNewFile *.h,*.c set filetype=c.doxygen 
augroup END


