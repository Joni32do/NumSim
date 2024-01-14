enum cell_type{
    fl_nn = 100,                 //fluid cell with no neighnours other than fluid
    
    fl_nt_border = 101,          //fluid cell with top border   
    fl_nr_border = 102,          //fluid cell with right border   
    fl_nb_border = 103,          //fluid cell with bottom border   
    fl_nl_border = 104,          //fluid cell with left border  

    fl_ntr_border = 105,          //fluid cell with top and right border   
    fl_nrb_border = 106,          //fluid cell with right and bottom border   
    fl_nbl_border = 107,          //fluid cell with bottom border   
    fl_nlt_border = 108,          //fluid cell with left border   

    fl_ntr_border = 105,          //fluid cell with top and right border   
    fl_nrb_border = 106,          //fluid cell with right and bottom border   
    fl_nbl_border = 107,          //fluid cell with bottom border   
    fl_nlt_border = 108,          //fluid cell with left border   
};