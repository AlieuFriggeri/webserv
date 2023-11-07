/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   DeleteRequestHandler.class.hpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afrigger <afrigger@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 10:44:20 by vgroux            #+#    #+#             */
/*   Updated: 2023/11/07 15:11:27 by afrigger         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DELETE_REQUEST_HANDLER_CLASS_HPP
# define DELETE_REQUEST_HANDLER_CLASS_HPP

#include "RequestHandler.class.hpp"

class DeleteRequestHandler: public RequestHandler
{
	private:

	public:
		DeleteRequestHandler(void);
		~DeleteRequestHandler(void);

		static HttpRespond	handleRequest(HttpRequest req);		
};

#endif