/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PostRequestHandler.class.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrigger <afrigger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:14:16 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 15:11:22 by afrigger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POST_REQUEST_HANDLER_CLASS_HPP
# define POST_REQUEST_HANDLER_CLASS_HPP

#include "RequestHandler.class.hpp"

class PostRequestHandler: public RequestHandler
{
	private:
		PostRequestHandler(void);

	public:
		PostRequestHandler(HttpRequest req);
		~PostRequestHandler(void);

		static HttpRespond	handleRequest(HttpRequest req);
};

#endif