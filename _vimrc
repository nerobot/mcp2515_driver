syntax on
colorscheme slate

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

<<<<<<< HEAD
set number
=======

>>>>>>> d08aa18bde3427084661c8b6c0d3c6db7d7a02a2
