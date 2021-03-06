#include <stdio.h>
#include <stdlib.h>
#include <linux/fs.h>
#include <linux/ext2_fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

#include "e2fs.h"

struct buffer
{
    void *data ;			/* les donnees du bloc */
    pblk_t blkno ;			/* numero de bloc physique */
    int valid ;				/* donnees valides */
    struct buffer *next ;
} ;

struct context
{
    int fd ;
    struct ext2_super_block sb ;
    int ngroups ;			/* nombre de groupes dans gd [] */
    struct ext2_group_desc *gd ;	/* c'est un tableau */
    /* ce qui suit concerne les lectures bufferisees */
    struct buffer *last ;		/* pointe sur dernier buffer */
    int bufstat_read ;			/* nombre de demandes de lecture */
    int bufstat_cached ;		/* nombre de lectures en cache */
} ;

struct ofile
{
    struct context *ctxt ;		/* eviter param a chaque e2_file_xxx */
    struct buffer *buffer ;		/* buffer contenant l'inode */
    struct ext2_inode *inode ;		/* l'inode proprement dit */
    lblk_t curblk ;			/* position en bloc */
    char *data ;			/* donnees */
    int len ;				/* longueur utile dans les donnees */
    int pos ;				/* position dans les donnees */
} ;

/******************************************************************************
 * Initialisation du contexte
 */

ctxt_t e2_ctxt_init (char *file, int maxbuf)
{
	int i=0;
	int fd_file;
	int nb_group;
	int block-size;
	buf_t tmp;
	ctxt_t c=malloc(sizeof(struct context));
	c->gd=malloc(sizeof(struct ext2_group_desc));

	if(fd_file==(open(file,O_RDONLY|O_WRONLY)==-1)){
		//to do: init errno
		return NULL;
	}
	
	lseek(fd_file,1024,SEEK_SET);
	
	if((read(fd_file,&c->sb,sizeof(struct ext2_super_block)))==-1){
		//to do: init errno
		return NULL;	
	}

	if(c->sb.s_magic!=EXT2_SUPER_MAGIC){
		//to do: init errno
		return NULL;
	}

	block_size=1024 << c->sb.s_log_block_size;
	nb_group=c->sb.s_blocks-count/c->sb.s_block_per_group;

	c->fd=fd_file;
	c->ngroup=nb_group;

	lseek(fd_file,1024+block_size,SEEK_SET);
	
	if((read(fd_file,c->gd,sizeof(struct ext2_group_desc)))==-1){
		//to do: init errno
		return NULL;	
	}


	c->bufstat_read=0;
	c->bufstat_cached=0;
	c->last=NULL;

	while(i<maxbuf){
		if(last==NULL){
			c->last=malloc(struct buffer);
			c->last->data=malloc(block_size);
			c->last->blkno=0;
			c_>last->valid=0;
			c->last->next=last;
		}
		else{
			tmp=c->last->next;
			c->last->next=malloc(struct buffer);
			c->last=next;
			c->last->data=malloc(block_size)
			c->last->blkno=0;
			c_>last->valid=0;
			c->last->next=tmp;
		} 
		i++;
	}

	return c;
}

void e2_ctxt_close (ctxt_t c)
{
	buf_t tmp;

	if(c!=NULL){
		close(c->fd);
		free(c->gd);
		free(c);

		while(c->last!=NULL){
			c->last->next=tmp;
			free(c->last->data);
			free(c->last);
			c->last=tmp;
		}
	}

}

int e2_ctxt_blksize (ctxt_t c)
{
	unsigned int block_size;
	
	if(c!=NULL){
		block_size=1024 << c->sb.s_log_block_size;
		return block_size;
	}
	else{
		return -1;
	}
}

/******************************************************************************
 * Fonctions de lecture non bufferisee d'un bloc
 */

int e2_block_fetch (ctxt_t c, pblk_t blkno, void *data)
{
	lseek(c->fd,1024+i*blkno,SEEK_SET);
	
	if((read(c->fd,data,e2_ctxt_blksize(c)))==-1){
		//to do: init errno
		return 0;	
	}
	else{
		return 1;
	}
}

/******************************************************************************
 * Gestion du buffer et lecture bufferisee
 */

/* recupere un buffer pour le bloc, le retire de la liste des buffers
 * et lit le contenu si necessaire
 */
buf_t e2_buffer_get (ctxt_t c, pblk_t blkno)
{

}
        
/* replace le buffer en premier dans la liste */
void e2_buffer_put (ctxt_t c, buf_t b)
{
}
        
/* recupere les donnees du buffer */
void *e2_buffer_data (buf_t b)
{
}

/* affiche les statistiques */
void e2_buffer_stats (ctxt_t c)
{
}

/******************************************************************************
 * Fonction de lecture d'un bloc dans un inode
 */

/* recupere le buffer contenant l'inode */
pblk_t e2_inode_to_pblk (ctxt_t c, inum_t i)
{
}

/* extrait l'inode du buffer */
struct ext2_inode *e2_inode_read (ctxt_t c, inum_t i, buf_t b)
{
}

/* numero de bloc physique correspondant au bloc logique blkno de l'inode in */
pblk_t e2_inode_lblk_to_pblk (ctxt_t c, struct ext2_inode *in, lblk_t blkno)
{
}

/******************************************************************************
 * Operation de haut niveau : affichage d'un fichier complet
 */

/* affiche les blocs d'un fichier */
int e2_cat (ctxt_t c, inum_t i, int disp_pblk)
{
}

/******************************************************************************
 * Simulation d'une ouverture de fichiers
 */

file_t e2_file_open (ctxt_t c, inum_t i)
{
}

void e2_file_close (file_t of)
{
}

/* renvoie EOF ou un caractere valide */
int e2_file_getc (file_t of)
{
}

/* renvoie nb de caracteres lus (0 lorsqu'on arrive en fin de fichier) */
int e2_file_read (file_t of, void *data, int len)
{
}

/******************************************************************************
 * Operations sur les repertoires
 */

/* retourne une entree de repertoire */
struct ext2_dir_entry_2 *e2_dir_get (file_t of)
{
}

/* recherche un composant de chemin dans un repertoire */
inum_t e2_dir_lookup (ctxt_t c, inum_t i, char *str, int len)
{
}

/******************************************************************************
 * Operation de haut niveau : affichage d'un repertoire complet
 */

/* affiche un repertoire donne par son inode */
int e2_ls (ctxt_t c, inum_t i)
{
}

/******************************************************************************
 * Traversee de repertoire
 */

/* recherche le fichier (l'inode) par son nom */
inum_t e2_namei (ctxt_t c, char *path)
{
}
