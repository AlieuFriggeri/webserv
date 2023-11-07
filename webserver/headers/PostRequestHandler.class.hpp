/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestHandler.class.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgroux <vgroux@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:14:16 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 17:37:55 by vgroux           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_REQUEST_HANDLER_CLASS_HPP
# define POST_REQUEST_HANDLER_CLASS_HPP

#include "RequestHandler.class.hpp"

class PostRequestHandler: public RequestHandler
{
	private:

	public:
		PostRequestHandler(void);
		// PostRequestHandler( CONFIG DU SERVER );
		PostRequestHandler(const PostRequestHandler &src);
		PostRequestHandler&	operator=(const PostRequestHandler &src);
		~PostRequestHandler(void);

		HttpRespond	handleRequest(HttpRequest req);
};

#endif